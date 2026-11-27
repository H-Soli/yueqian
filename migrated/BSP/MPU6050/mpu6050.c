#include "board.h"
#include "mpu6050.h"
#include "mpu_port.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <math.h>

#define PACKET_BYTES 28U /* 16 quaternion + 6 accel + 6 calibrated gyro */
volatile unsigned int mpu_status=MPU_OFF;
volatile int mpu_error_code=0,mpu_yaw_sign=1;
volatile float mpu_yaw_deg=0,mpu_rate_dps=0;
volatile uint32_t mpu_sample_ms=0,mpu_epoch=0;
volatile uint8_t mpu_id_68=0,mpu_id_69=0,mpu_probe_68=9,mpu_probe_69=9;
static volatile unsigned int reset_pending=0,bus_busy=0;
static unsigned int initialized=0,quiet_samples=0;
static float gyro_sensitivity=16.4f;

void mpu_request_fifo_reset(void){reset_pending=1;}
static int init_error(int code)
{
 mpu_status=MPU_ERROR;mpu_error_code=code;initialized=0;return code;
}
static uint8_t probe(uint8_t address,volatile uint8_t *id)
{
 uint8_t value=0;unsigned int attempt;
 for(attempt=0;attempt<3;attempt++){
  if(!mpu_port_read(address,0x75,1,&value)){*id=value;return 0;}
  delay_ms(5);
 }
 *id=0;return (uint8_t)mpu_bus_fault;
}
int mpu6050_init(void)
{
 initialized=0;quiet_samples=0;reset_pending=0;mpu_epoch++;
 mpu_status=MPU_CAL;mpu_error_code=0;
 /* Probe both AD0 states. WHO_AM_I is 0x68 for MPU6050 at either address.
  * Keep ID and bus failures separate; never accept another chip silently. */
 mpu_i2c_address=0x68;
 mpu_port_sample_lines();
 mpu_probe_68=probe(0x68,&mpu_id_68);
 mpu_probe_69=probe(0x69,&mpu_id_69);
 if(!mpu_probe_68 && mpu_id_68==0x68)mpu_i2c_address=0x68;
 else if(!mpu_probe_69 && mpu_id_69==0x68)mpu_i2c_address=0x69;
 else return init_error((!mpu_probe_68 || !mpu_probe_69)?-31:-30);
 if(mpu_init())return init_error(-2);
 if(mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL))return init_error(-3);
 if(mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL))return init_error(-4);
 if(mpu_set_sample_rate(100))return init_error(-5);
 if(dmp_load_motion_driver_firmware())return init_error(-6);
 /* {-1,0,0; 0,-1,0; 0,0,1} -> 4 | (5<<3) | (2<<6). */
 if(dmp_set_orientation(172))return init_error(-7);
 if(dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_SEND_RAW_ACCEL|
    DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL))return init_error(-8);
 if(dmp_set_fifo_rate(100) || dmp_set_interrupt_mode(DMP_INT_CONTINUOUS))return init_error(-9);
 if(mpu_set_int_level(1) || mpu_set_int_latched(0))return init_error(-10);
 if(mpu_get_gyro_sens(&gyro_sensitivity) || gyro_sensitivity<=0)return init_error(-11);
 if(mpu_set_dmp_state(1))return init_error(-12);
 initialized=1;return 0;
}
unsigned int mpu6050_valid(uint32_t now)
{
 return initialized && mpu_status==MPU_READY && !reset_pending && !bus_busy &&
        (uint32_t)(now-mpu_sample_ms)<=80U;
}
void mpu6050_poll(void)
{
 uint8_t count_data[2],more=0;unsigned int count,i;
 short gyro[3],accel[3],sensors=0;
 long quat[4];unsigned long timestamp;
 float q0,q1,q2,q3,rate,norm;
 if(!initialized || reset_pending || bus_busy)return;
 if(mpu_port_read(0x68,0x72,2,count_data))goto failed;
 count=((unsigned int)count_data[0]<<8)|count_data[1];
 if(count<PACKET_BYTES){
  if(mpu_status==MPU_READY && (uint32_t)(uwTick-mpu_sample_ms)>80U){
   mpu_status=MPU_ERROR;mpu_error_code=-22;
  }
  return; /* no new sample is normal, until its freshness deadline */
 }
 if(count>3U*PACKET_BYTES){mpu_request_fifo_reset();return;}
 /* Drain at most three packets, publishing only the newest orientation. */
 for(i=0;i<3;i++){
  if(dmp_read_fifo(gyro,accel,quat,&timestamp,&sensors,&more))goto failed;
  if(!more)break;
 }
 if(more || !(sensors&INV_WXYZ_QUAT))goto failed;
 q0=quat[0]/1073741824.0f;q1=quat[1]/1073741824.0f;
 q2=quat[2]/1073741824.0f;q3=quat[3]/1073741824.0f;
 norm=q0*q0+q1*q1+q2*q2+q3*q3;
 if(!isfinite(norm) || norm<0.8f || norm>1.2f)goto failed;
 rate=(float)gyro[2]/gyro_sensitivity;
 mpu_yaw_deg=(mpu_yaw_sign<0?-1.0f:1.0f)*
  atan2f(2.0f*(q1*q2+q0*q3),q0*q0+q1*q1-q2*q2-q3*q3)*57.2957795f;
 mpu_rate_dps=(mpu_yaw_sign<0?-1.0f:1.0f)*rate;
 mpu_sample_ms=uwTick;
 /* The supplied DMP gyro calibration needs eight seconds without motion.
  * Motors remain off during the startup polling interval in main. */
 if(quiet_samples<800){
  if(fabsf(rate)<3.0f)quiet_samples++;else quiet_samples=0;
 }
 mpu_status=quiet_samples>=800?MPU_READY:MPU_CAL;
 mpu_error_code=0;return;
failed:
 mpu_error_code=-20;mpu_status=MPU_ERROR;mpu_request_fifo_reset();
}
void mpu6050_background(void)
{
 uint32_t mask;int result;
 if(!initialized || !reset_pending)return;
 /* Main owns I2C during recovery; timer polling skips while bus_busy. */
 mask=__get_PRIMASK();__disable_irq();bus_busy=1;__set_PRIMASK(mask);
 result=mpu_reset_fifo();
 mpu_epoch++; /* Never apply an old gap target after a DMP/FIFO reset. */
 reset_pending=0;
 if(result){mpu_status=MPU_ERROR;mpu_error_code=-21;initialized=0;}
 else {mpu_status=MPU_CAL;quiet_samples=0;}
 bus_busy=0;
}

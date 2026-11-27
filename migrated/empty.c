#include "board.h"
#include "key.h"
#include "mpu6050.h"
#include "mpu_port.h"
int main(void)
{
 char text[24];uint32_t last=0;
 board_init();delay_ms(100);OLED_Init();OLED_Clear();
 OLED_ShowString(0,0,(uint8_t*)"D11 MPU6050",16,1);
 OLED_ShowString(0,16,(uint8_t*)"Keep still",16,1);OLED_Refresh();
 if(mpu6050_init()==0){
  uint32_t start=uwTick;
  while((uint32_t)(uwTick-start)<10000U && mpu_status!=MPU_READY){
   mpu6050_poll();mpu6050_background();delay_ms(5);
  }
 }
 OLED_Clear();board_start_interrupts();Gyro_ConfigReportRateRx();
 while(1){
  mpu6050_background();
  if((uint32_t)(uwTick-last)>=100U){
   uint8_t raw=xunji_raw_mask,i;char state,imu;
   last=uwTick;
   snprintf(text,sizeof(text),"L:%6d",filt_velocity_l);
   OLED_ShowString(0,0,(uint8_t*)text,16,1);
   snprintf(text,sizeof(text),"R:%6d",filt_velocity_r);
   OLED_ShowString(0,16,(uint8_t*)text,16,1);
   /* Y/T are in tenths of a degree; no float printf on the small MCU. */
   if(mpu_status==MPU_ERROR)
    snprintf(text,sizeof(text),"68:%02X/%u 69:%02X/%u  ",mpu_id_68,mpu_probe_68,mpu_id_69,mpu_probe_69);
   else snprintf(text,sizeof(text),"Y:%5d T:%5d  ",(int)(mpu_yaw_deg*10.0f),(int)(heading_target*10.0f));
   OLED_ShowString(0,32,(uint8_t*)text,8,1);
   if(mpu_status==MPU_ERROR)
    snprintf(text,sizeof(text),"P:%u ST:%04lX B:%u  ",mpu_fault_phase,
             (unsigned long)mpu_fault_status,mpu_bus_fault);
   else snprintf(text,sizeof(text),"PWM L:%3d R:%3d  ",motor_command_l,motor_command_r);
   OLED_ShowString(0,40,(uint8_t*)text,8,1);
   state=flag==1?(track_has_line?(xunji_centered?'C':'T'):'G'):'S';
   imu=mpu_status==MPU_READY?'R':(mpu_status==MPU_CAL?'C':'E');
   snprintf(text,sizeof(text),"D11 M:%d %c%c I:%c ",flag,state,
            flag==1 && !track_has_line?(gap_heading_active?'H':'F'):' ',imu);
   OLED_ShowString(0,48,(uint8_t*)text,8,1);
   if(mpu_status==MPU_ERROR)
    snprintf(text,sizeof(text),"N:%u%u U:%u%u E:%d ",
     (mpu_idle_levels>>1)&1U,mpu_idle_levels&1U,
     (mpu_pullup_levels>>1)&1U,mpu_pullup_levels&1U,mpu_error_code);
   else{
    memcpy(text,"IO:",3);
    for(i=0;i<8;i++)text[3+i]=(raw&(0x80U>>i))?'1':'0';
    snprintf(text+11,sizeof(text)-11," E:%3d  ",mpu_error_code);
   }
   OLED_ShowString(0,56,(uint8_t*)text,8,1);OLED_Refresh();
  }
 }
}

#ifndef MPU6050_H
#define MPU6050_H
#include <stdint.h>
enum {MPU_OFF=0,MPU_CAL=1,MPU_READY=2,MPU_ERROR=3};
extern volatile unsigned int mpu_status;
extern volatile int mpu_error_code;
extern volatile float mpu_yaw_deg,mpu_rate_dps;
/* Original installation: Z up, positive yaw = left turn. Set -1 only if
 * the real installation reverses the observed yaw direction. */
extern volatile int mpu_yaw_sign;
extern volatile uint32_t mpu_sample_ms,mpu_epoch;
extern volatile uint8_t mpu_id_68,mpu_id_69,mpu_probe_68,mpu_probe_69;
int mpu6050_init(void);
void mpu6050_poll(void); /* 10 ms, control IRQ; bounded FIFO work */
void mpu6050_background(void); /* main only: deferred FIFO recovery */
unsigned int mpu6050_valid(uint32_t now);
#endif

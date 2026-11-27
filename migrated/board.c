#include "board.h"
#include "key.h"
#include "heading.h"
#include "mpu6050.h"
volatile uint32_t uwTick=0,control_cycles=0;
volatile int flag=0;
volatile int filt_velocity_l=0,filt_velocity_r=0;
volatile float cur_angle_display=0,heading_target=0;
volatile uint8_t track_has_line=0,gap_heading_active=0;
volatile float gap_heading_error=0,gap_heading_correction=0;
extern volatile int _encoder_l_count,_encoder_r_count;
void board_init(void)
{
 SYSCFG_DL_init();Set_Pwm(0,0);
 NVIC_SetPriority(UART_1_INST_INT_IRQN,1);
 NVIC_SetPriority(TIMER_A1_KONG_ZHI_INST_INT_IRQN,2);
 /* External IRQs start after OLED/MPU initialization; SysTick runs now. */
}
void board_start_interrupts(void)
{
 DL_TimerA_clearInterruptStatus(TIMER_A1_KONG_ZHI_INST,DL_TIMERA_INTERRUPT_ZERO_EVENT);
 DL_GPIO_clearInterruptStatus(MPU6050_PORT,MPU6050_INT_PIN_PIN);
 NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);NVIC_EnableIRQ(GPIOB_INT_IRQn);
 NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);NVIC_EnableIRQ(UART_1_INST_INT_IRQN);
 NVIC_ClearPendingIRQ(TIMER_A1_KONG_ZHI_INST_INT_IRQN);NVIC_EnableIRQ(TIMER_A1_KONG_ZHI_INST_INT_IRQN);
}
static int limit_pwm(int pwm)
{
 if(pwm>40)return 40;if(pwm< -30)return -30;return pwm;
}
void TIMA1_IRQHandler(void)
{
 static int previous_mode=0;
 static uint8_t line_seen=0,in_gap=0;
 static HeadingHold gap;
 static uint32_t gap_epoch=0,mpu_last_poll=0,heading_update_ms=0;
 static uint32_t gyro_last_ms=0;
 static int16_t last_gyro=0;
 float correction;
 if(DL_TimerA_getPendingInterrupt(TIMER_A1_KONG_ZHI_INST)!=DL_TIMER_IIDX_ZERO)return;
 control_cycles++;
 /* Read MPU independently of the slow OLED refresh in main. */
 if((uint32_t)(uwTick-mpu_last_poll)>=10U){mpu_last_poll=uwTick;mpu6050_poll();}
 /* Preserve UART gyro and the existing optional flag=2 test mode. */
 {uint32_t mask=__get_PRIMASK();__disable_irq();
  if(gyro_rx_done){gyro_rx_done=0;gyro_last_ms=uwTick;last_gyro=gyro_angle_raw;}
  __set_PRIMASK(mask);
 }
 if(flag!=previous_mode){
  place_PID_reset();angle_err_sum=0;angle_last_err=0;
  if(flag==2)heading_target=(float)last_gyro*0.1f;
  line_seen=0;in_gap=0;heading_update_ms=uwTick-25U;
  gap_heading_active=0;gap_heading_error=0;gap_heading_correction=0;
  previous_mode=flag;
 }
 track_has_line=xunji_refresh();
 if(flag==1){
  if(track_has_line){
   if(!line_seen)place_PID_reset();
   line_seen=1;in_gap=0;
   gap_heading_active=0;gap_heading_error=0;gap_heading_correction=0;
   if(xunji_centered){place_PID_reset();correction=0;}
   else correction=place_PID_value(0.0f,xun);
   if(correction>15.0f)correction=15.0f;
   if(correction< -15.0f)correction= -15.0f;
  }else{
   unsigned int valid=mpu6050_valid(uwTick);
   if(!in_gap){
    /* Freeze the exit heading ONCE for this entire unmarked segment. */
    heading_begin(&gap,mpu_yaw_deg,valid);
    heading_target=gap.target;gap_epoch=mpu_epoch;in_gap=1;
   }
   valid=valid && gap_epoch==mpu_epoch;
   gap_heading_active=valid && gap.locked;
   gap_heading_error=gap_heading_active?heading_error(gap.target,mpu_yaw_deg):0;
   correction=heading_step(&gap,mpu_yaw_deg,mpu_rate_dps,valid);
   gap_heading_correction=correction;line_seen=0;place_PID_reset();
  }
  /* Positive yaw/error = LEFT turn, matching archived installation.
   * Set_Pwm order is RIGHT,LEFT. Gap outputs stay forward at 22..38.
   * Invalid IMU data clears steering; it never causes line-loss stopping. */
  Set_Pwm(limit_pwm((int)(30.0f+correction)),limit_pwm((int)(30.0f-correction)));
 }else if(flag==2){
  if((uint32_t)(uwTick-heading_update_ms)<25U)return;
  heading_update_ms=uwTick;
  if(!gyro_last_ms || (uint32_t)(uwTick-gyro_last_ms)>250U){Set_Pwm(0,0);return;}
  correction=angle_PID_value(heading_error(heading_target,(float)last_gyro*0.1f),0.0f);
  Set_Pwm(limit_pwm((int)(35.0f+correction)),limit_pwm((int)(35.0f-correction)));
 }else Set_Pwm(0,0);
}
void SysTick_Handler(void)
{
 static float filtered_l=0,filtered_r=0;
 static uint32_t last=0;int left,right;uint32_t mask;
 uwTick++;key_scan_1ms();
 if(uwTick-last>10U){
  last=uwTick;mask=__get_PRIMASK();__disable_irq();
  left=_encoder_l_count;right=_encoder_r_count;
  _encoder_l_count=0;_encoder_r_count=0;__set_PRIMASK(mask);
  filtered_l=0.3f*left+0.7f*filtered_l;
  filtered_r=0.3f*right+0.7f*filtered_r;
  filt_velocity_l=(int)(filtered_l+(filtered_l<0?-0.5f:0.5f));
  filt_velocity_r=(int)(filtered_r+(filtered_r<0?-0.5f:0.5f));
 }
}
void UART_1_INST_IRQHandler(void)
{
 switch(DL_UART_getPendingInterrupt(UART_1_INST)){
 case DL_UART_IIDX_RX:Gyro_ParseFrame(DL_UART_Main_receiveData(UART_1_INST));break;
 default:break;
 }
}
void delay_us(unsigned long us)
{
 while(us--){uint32_t start=SysTick->VAL,elapsed=0,prev=start;
  while(elapsed<CPUCLK_FREQ/1000000U){uint32_t now=SysTick->VAL;
   elapsed+=prev>=now?prev-now:SysTick->LOAD+1U-now+prev;prev=now;
  }
 }
}
void delay_ms(unsigned long ms)
{
 uint32_t start=uwTick;while((uint32_t)(uwTick-start)<ms)__NOP();
}
void delay_1us(unsigned long us){delay_us(us);}
void delay_1ms(unsigned long ms){delay_ms(ms);}

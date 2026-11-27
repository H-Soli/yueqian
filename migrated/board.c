#include "board.h"
#include "key.h"
volatile uint32_t uwTick=0;
volatile int flag=0;
volatile int filt_velocity_l=0,filt_velocity_r=0;
volatile float cur_angle_display=0;
extern volatile int _encoder_l_count,_encoder_r_count;
extern uint8_t l1,l2,l3,l4,r1,r2,r3,r4;
void board_init(void)
{
 SYSCFG_DL_init();Set_Pwm(0,0);
 NVIC_SetPriority(UART_1_INST_INT_IRQN,1);
 NVIC_SetPriority(TIMER_A1_KONG_ZHI_INST_INT_IRQN,2);
 NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);NVIC_EnableIRQ(GPIOB_INT_IRQn);
 NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);NVIC_EnableIRQ(UART_1_INST_INT_IRQN);
 NVIC_ClearPendingIRQ(TIMER_A1_KONG_ZHI_INST_INT_IRQN);NVIC_EnableIRQ(TIMER_A1_KONG_ZHI_INST_INT_IRQN);
}


/* Preserve the latest source's output mapping: right=35-2*PID, left=35+2*PID.
 * flag 0: stop; flag 1: line tracking; flag 2: gyro heading hold.
 * Heading-hold target is captured when entering mode 2; turn correction is
 * bench-test dependent. No encoder speed PID is enabled implicitly.
 */
volatile float heading_target=0;
static int limit_pwm(int pwm){if(pwm>40)return 40;if(pwm< -30)return -30;return pwm;}
void TIMA1_IRQHandler(void)
{
 static int previous_mode=0;
 static uint32_t gyro_last_ms=0;
 static int16_t last_gyro=0;
 if(DL_TimerA_getPendingInterrupt(TIMER_A1_KONG_ZHI_INST)!=DL_TIMER_IIDX_ZERO)return;
 if(gyro_rx_done){gyro_rx_done=0;gyro_last_ms=uwTick;last_gyro=gyro_angle_raw;}
 if(flag!=previous_mode){
 place_err_sum=0;place_last_err=0;angle_err_sum=0;angle_last_err=0;
 if(flag==2)heading_target=(float)last_gyro*0.1f;
 previous_mode=flag;
 }
 if(flag==1){
 if(xunji_refresh()){
 float correction=place_PID_value(0.0f,xun);
 int right=limit_pwm((int)(35.0f-correction*2.0f));
 int left=limit_pwm((int)(35.0f+correction*2.0f));
 Set_Pwm(right,left);
 }else{Set_Pwm(0,0);place_err_sum=0;place_last_err=0;}
 }else if(flag==2){
 if(!gyro_last_ms || (uint32_t)(uwTick-gyro_last_ms)>250){Set_Pwm(0,0);return;}
 float current=(float)last_gyro*0.1f;
 float err=heading_target-current;
 while(err>180.0f)err-=360.0f;
 while(err< -180.0f)err+=360.0f;
 float correction=angle_PID_value(err,0.0f);
 Set_Pwm(limit_pwm((int)(35.0f+correction)),limit_pwm((int)(35.0f-correction)));
 }else{Set_Pwm(0,0);}
}

void SysTick_Handler(void)
{
 static uint32_t last=0;int left,right;uint32_t mask;
 uwTick++;
 key_scan_1ms();
 if(uwTick-last>10){
 last=uwTick;mask=__get_PRIMASK();__disable_irq();
 left=_encoder_l_count;right=_encoder_r_count;
 _encoder_l_count=0;_encoder_r_count=0;__set_PRIMASK(mask);
 filt_velocity_l=(int)(0.3f*left+0.7f*filt_velocity_l);
 filt_velocity_r=(int)(0.3f*right+0.7f*filt_velocity_r);
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
 elapsed+=prev>=now?prev-now:SysTick->LOAD+1U-now+prev;prev=now;}
 }
}
void delay_ms(unsigned long ms){uint32_t start=uwTick;while((uint32_t)(uwTick-start)<ms){__NOP();}}
void delay_1us(unsigned long us){delay_us(us);}
void delay_1ms(unsigned long ms){delay_ms(ms);}

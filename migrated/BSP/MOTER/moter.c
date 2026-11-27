#include "board.h"
void PWM_Xianfu(int max,int *pwm){if(*pwm>max)*pwm=max;if(*pwm< -max)*pwm= -max;}
/* Legacy API order: RIGHT, LEFT. Keep original pin mapping and direction. */
void Set_Pwm(int right,int left)
{
 PWM_Xianfu(100,&right); PWM_Xianfu(100,&left);
 if(left<0){Zuo_1_ON;Zuo_2_OFF;}else{Zuo_1_OFF;Zuo_2_ON;}
 if(right<0){Yuo_1_ON;Yuo_2_OFF;}else{Yuo_1_OFF;Yuo_2_ON;}
 DL_TimerA_setCaptureCompareValue(PWMAB_INST,left<0?-left:left,GPIO_PWMAB_C0_IDX);
 DL_TimerA_setCaptureCompareValue(PWMAB_INST,right<0?-right:right,GPIO_PWMAB_C1_IDX);
 /* Force both direction inputs low on zero command for a deterministic stop. */
 if(left==0){Zuo_1_OFF;Zuo_2_OFF;}
 if(right==0){Yuo_1_OFF;Yuo_2_OFF;}
}

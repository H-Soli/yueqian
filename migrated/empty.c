#include "board.h"
/* Power-on stop; debounced key0 (PA23) starts line tracking. */
int main(void)
{
 char text[24];uint32_t last=0;
 board_init();delay_ms(100);OLED_Init();OLED_Clear();
 Gyro_ConfigReportRateRx();
 while(1){
 if((uint32_t)(uwTick-last)>=100){last=uwTick;
 snprintf(text,sizeof(text),"L:%6d",filt_velocity_l);OLED_ShowString(0,0,(uint8_t*)text,16,1);
 snprintf(text,sizeof(text),"R:%6d",filt_velocity_r);OLED_ShowString(0,16,(uint8_t*)text,16,1);
 snprintf(text,sizeof(text),"Yaw:%6d",(int)gyro_angle_raw);OLED_ShowString(0,32,(uint8_t*)text,16,1);
 snprintf(text,sizeof(text),"Mode:%d  ",flag);OLED_ShowString(0,48,(uint8_t*)text,16,1);
 OLED_Refresh();}
 }
}

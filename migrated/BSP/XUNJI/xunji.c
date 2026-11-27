#include "xunji.h"
uint8_t l4,l3,l2,l1,r1,r2,r3,r4;
volatile float xun=0,last_xun=0;
volatile uint8_t lost_cnt=0;
volatile uint8_t xunji_black_level=XUNJI_BLACK_LEVEL_DEFAULT;
volatile uint8_t xunji_raw_mask=0,xunji_active_mask=0;
volatile uint8_t xunji_centered=0;
float z=0;
uint8_t my_GPIO_readPin(GPIO_Regs *gpio,uint32_t pins)
{
 return DL_GPIO_readPins(gpio,pins)!=0;
}
uint8_t xunji_refresh(void)
{
 /* Read each port once for a consistent eight-channel snapshot. */
 uint32_t a=GPIOA->DIN31_0,b=GPIOB->DIN31_0;
 uint8_t raw=0,active;
 if(b&GPIO_XUNJI_L4_PIN)raw|=0x80;
 if(b&GPIO_XUNJI_L3_PIN)raw|=0x40;
 if(a&GPIO_XUNJI_L2_PIN)raw|=0x20;
 if(a&GPIO_XUNJI_L1_PIN)raw|=0x10;
 if(b&GPIO_XUNJI_R1_PIN)raw|=0x08;
 if(b&GPIO_XUNJI_R2_PIN)raw|=0x04;
 if(a&GPIO_XUNJI_R3_PIN)raw|=0x02;
 if(a&GPIO_XUNJI_R4_PIN)raw|=0x01;
 active=xunji_black_level?raw:(uint8_t)~raw;
 xunji_raw_mask=raw;xunji_active_mask=active;
 /* Only L1, only R1, or L1+R1 means straight ahead. Outer sensors still
  * request steering; zero active sensors still means a genuine gap. */
 xunji_centered=(active==0x10U || active==0x08U || active==0x18U);
 l4=(active>>7)&1U;l3=(active>>6)&1U;l2=(active>>5)&1U;l1=(active>>4)&1U;
 r1=(active>>3)&1U;r2=(active>>2)&1U;r3=(active>>1)&1U;r4=active&1U;
 z=l4+l3+l2+l1+r1+r2+r3+r4;
 if(z==0){xun=0;if(lost_cnt<255)lost_cnt++;return 0;}
 lost_cnt=0;
 if(xunji_centered)xun=0;
 else xun=(-3.5f*l4-2.5f*l3-1.5f*l2-0.5f*l1+
       0.5f*r1+1.5f*r2+2.5f*r3+3.5f*r4)/z;
 last_xun=xun;return 1;
}

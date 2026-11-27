#include "xunji.h"
#include "ti_msp_dl_config.h"
#include "board.h"

uint8_t l4=0, l3=0, l2=0, l1=0, r1=0, r2=0, r3=0, r4=0;

	


volatile float xun; 
volatile float last_xun = 0.0f;
volatile uint8_t  lost_cnt = 0;

static uint16_t find_set_bit(uint32_t num) 
{
    uint8_t bit_position = 0;
    while (!(num & 1)) {
        num >>= 1;
        bit_position++;
    }
    return bit_position;
}
uint8_t my_GPIO_readPin(GPIO_Regs * 	gpio,uint32_t 	pins )	
{
	uint16_t bit_position;
	uint32_t read_32;
	bit_position=find_set_bit(pins);
	read_32=DL_GPIO_readPins(gpio,pins);
	return ((read_32>>bit_position)&1);
}

float z = 0;   
uint8_t xunji_refresh()
{    
      z = 0;

		
		if(0 < my_GPIO_readPin(GPIO_XUNJI_L4_PORT, GPIO_XUNJI_L4_PIN))  {l4=1;z++;}else{l4=0;}  
		if(0 < my_GPIO_readPin(GPIO_XUNJI_L3_PORT, GPIO_XUNJI_L3_PIN))  {l3=1;z++;}else{l3=0;}  
		if(0 < my_GPIO_readPin(GPIO_XUNJI_L2_PORT, GPIO_XUNJI_L2_PIN))  {l2=1;z++;}else{l2=0;}  
		if(0 < my_GPIO_readPin(GPIO_XUNJI_L1_PORT, GPIO_XUNJI_L1_PIN))  {l1=1;z++;}else{l1=0;}  
		if(0 < my_GPIO_readPin(GPIO_XUNJI_R1_PORT, GPIO_XUNJI_R1_PIN))  {r1=1;z++;}else{r1=0;}  
		if(0 < my_GPIO_readPin(GPIO_XUNJI_R2_PORT, GPIO_XUNJI_R2_PIN))  {r2=1;z++;}else{r2=0;}  
		if(0 < my_GPIO_readPin(GPIO_XUNJI_R3_PORT, GPIO_XUNJI_R3_PIN))  {r3=1;z++;}else{r3=0;}  
		if(0 < my_GPIO_readPin(GPIO_XUNJI_R4_PORT, GPIO_XUNJI_R4_PIN))  {r4=1;z++;}else{r4=0;}  

        if(z > 0)  
        {
            xun = (l4* -3.5f + l3* -2.5f + l2* -1.5f + l1* -0.5f + r1* 0.5f + r2* 1.5f + r3* 2.5f + r4* 3.5f) / z;
            last_xun = xun;
					return 1;
        }
        else  
        {
						return 0;
        } 
    }





















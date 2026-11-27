#include "board.h"
#include "key.h"
volatile unsigned int key0_level=1;
/* Both press and release must remain stable for 20 samples. */
void key_scan_1ms(void)
{
 static uint8_t stable=0, count=0;
 uint8_t pressed;
 key0_level=DL_GPIO_readPins(KEY_PORT,KEY_KEY1_PIN_A23_PIN)?1U:0U;
 pressed=(key0_level==0);
 if(pressed==stable){count=0;return;}
 if(++count>=20){
 count=0;stable=pressed;
 if(stable)flag=1;
 }
}

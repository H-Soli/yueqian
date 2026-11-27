#ifndef XUNJI_H_
#define XUNJI_H_
#include "../../ti_msp_dl_config.h"
/* Original polarity retained pending bench measurement: 1=black HIGH. */
#define XUNJI_BLACK_LEVEL_DEFAULT 1U
extern volatile uint8_t xunji_black_level;
/* bit7..0 = L4 L3 L2 L1 R1 R2 R3 R4; raw levels, not inverted. */
extern volatile uint8_t xunji_raw_mask,xunji_active_mask;
/* A narrow line need only cover ONE of the two middle sensors. */
extern volatile uint8_t xunji_centered;
extern volatile float last_xun;
uint8_t xunji_refresh(void);
uint8_t my_GPIO_readPin(GPIO_Regs *gpio,uint32_t pins);
#endif

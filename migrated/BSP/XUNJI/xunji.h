#ifndef XUNJI_H_
#define XUNJI_H_

#include "../../ti_msp_dl_config.h"






uint8_t xunji_refresh();
int xunji_sign();//检测到黑线，返回0,检测到全白，返回1

extern volatile float last_xun;


#endif
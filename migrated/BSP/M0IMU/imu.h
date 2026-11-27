#ifndef CAR_IMU_H
#define CAR_IMU_H
#include "ti_msp_dl_config.h"
void Gyro_ParseFrame(uint8_t data);
void Gyro_ConfigReportRateRx(void);
#endif

#ifndef CAR_BOARD_H
#define CAR_BOARD_H
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "moter.h"
#include "pid.h"
#include "encoder.h"
#include "xunji.h"
#include "imu.h"
#include "oled.h"
void board_init(void);
void delay_us(unsigned long us);
void delay_ms(unsigned long ms);
void delay_1us(unsigned long us);
void delay_1ms(unsigned long ms);
extern volatile int flag; /* 0=stop, 1=track, 2=gyro heading hold */
extern volatile uint32_t uwTick;
extern volatile int filt_velocity_l, filt_velocity_r; /* filtered counts per 11 ms, NOT RPM */
extern volatile int16_t gyro_angle_raw, gyro_dps_raw;
extern volatile uint8_t gyro_rx_done;
extern float place_last_err;
extern volatile float heading_target;
extern volatile float xun;
#endif

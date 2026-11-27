/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0G3507
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for PWMAB */
#define PWMAB_INST                                                         TIMA0
#define PWMAB_INST_IRQHandler                                   TIMA0_IRQHandler
#define PWMAB_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define PWMAB_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 0 */
#define GPIO_PWMAB_C0_PORT                                                 GPIOB
#define GPIO_PWMAB_C0_PIN                                         DL_GPIO_PIN_14
#define GPIO_PWMAB_C0_IOMUX                                      (IOMUX_PINCM31)
#define GPIO_PWMAB_C0_IOMUX_FUNC                     IOMUX_PINCM31_PF_TIMA0_CCP0
#define GPIO_PWMAB_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWMAB_C1_PORT                                                 GPIOA
#define GPIO_PWMAB_C1_PIN                                          DL_GPIO_PIN_7
#define GPIO_PWMAB_C1_IOMUX                                      (IOMUX_PINCM14)
#define GPIO_PWMAB_C1_IOMUX_FUNC                     IOMUX_PINCM14_PF_TIMA0_CCP1
#define GPIO_PWMAB_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for TIMER_A1_KONG_ZHI */
#define TIMER_A1_KONG_ZHI_INST                                           (TIMA1)
#define TIMER_A1_KONG_ZHI_INST_IRQHandler                        TIMA1_IRQHandler
#define TIMER_A1_KONG_ZHI_INST_INT_IRQN                         (TIMA1_INT_IRQn)
#define TIMER_A1_KONG_ZHI_INST_LOAD_VALUE                                 (2499U)



/* Defines for UART_1 */
#define UART_1_INST                                                        UART1
#define UART_1_INST_FREQUENCY                                            4000000
#define UART_1_INST_IRQHandler                                  UART1_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                         DL_GPIO_PIN_9
#define GPIO_UART_1_TX_PIN                                         DL_GPIO_PIN_8
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM20)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM19)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM20_PF_UART1_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM19_PF_UART1_TX
#define UART_1_BAUD_RATE                                                (115200)
#define UART_1_IBRD_4_MHZ_115200_BAUD                                        (2)
#define UART_1_FBRD_4_MHZ_115200_BAUD                                       (11)





/* Port definition for Pin Group TB6612 */
#define TB6612_PORT                                                      (GPIOB)

/* Defines for ZUO_IN1: GPIOB.9 with pinCMx 26 on package pin 61 */
#define TB6612_ZUO_IN1_PIN                                       (DL_GPIO_PIN_9)
#define TB6612_ZUO_IN1_IOMUX                                     (IOMUX_PINCM26)
/* Defines for ZUO_IN2: GPIOB.10 with pinCMx 27 on package pin 62 */
#define TB6612_ZUO_IN2_PIN                                      (DL_GPIO_PIN_10)
#define TB6612_ZUO_IN2_IOMUX                                     (IOMUX_PINCM27)
/* Defines for YOU_IN1: GPIOB.7 with pinCMx 24 on package pin 59 */
#define TB6612_YOU_IN1_PIN                                       (DL_GPIO_PIN_7)
#define TB6612_YOU_IN1_IOMUX                                     (IOMUX_PINCM24)
/* Defines for YOU_IN2: GPIOB.6 with pinCMx 23 on package pin 58 */
#define TB6612_YOU_IN2_PIN                                       (DL_GPIO_PIN_6)
#define TB6612_YOU_IN2_IOMUX                                     (IOMUX_PINCM23)
/* Defines for L4: GPIOB.19 with pinCMx 45 on package pin 16 */
#define GPIO_XUNJI_L4_PORT                                               (GPIOB)
#define GPIO_XUNJI_L4_PIN                                       (DL_GPIO_PIN_19)
#define GPIO_XUNJI_L4_IOMUX                                      (IOMUX_PINCM45)
/* Defines for L3: GPIOB.17 with pinCMx 43 on package pin 14 */
#define GPIO_XUNJI_L3_PORT                                               (GPIOB)
#define GPIO_XUNJI_L3_PIN                                       (DL_GPIO_PIN_17)
#define GPIO_XUNJI_L3_IOMUX                                      (IOMUX_PINCM43)
/* Defines for L2: GPIOA.16 with pinCMx 38 on package pin 9 */
#define GPIO_XUNJI_L2_PORT                                               (GPIOA)
#define GPIO_XUNJI_L2_PIN                                       (DL_GPIO_PIN_16)
#define GPIO_XUNJI_L2_IOMUX                                      (IOMUX_PINCM38)
/* Defines for L1: GPIOA.14 with pinCMx 36 on package pin 7 */
#define GPIO_XUNJI_L1_PORT                                               (GPIOA)
#define GPIO_XUNJI_L1_PIN                                       (DL_GPIO_PIN_14)
#define GPIO_XUNJI_L1_IOMUX                                      (IOMUX_PINCM36)
/* Defines for R1: GPIOB.20 with pinCMx 48 on package pin 19 */
#define GPIO_XUNJI_R1_PORT                                               (GPIOB)
#define GPIO_XUNJI_R1_PIN                                       (DL_GPIO_PIN_20)
#define GPIO_XUNJI_R1_IOMUX                                      (IOMUX_PINCM48)
/* Defines for R2: GPIOB.25 with pinCMx 56 on package pin 27 */
#define GPIO_XUNJI_R2_PORT                                               (GPIOB)
#define GPIO_XUNJI_R2_PIN                                       (DL_GPIO_PIN_25)
#define GPIO_XUNJI_R2_IOMUX                                      (IOMUX_PINCM56)
/* Defines for R3: GPIOA.25 with pinCMx 55 on package pin 26 */
#define GPIO_XUNJI_R3_PORT                                               (GPIOA)
#define GPIO_XUNJI_R3_PIN                                       (DL_GPIO_PIN_25)
#define GPIO_XUNJI_R3_IOMUX                                      (IOMUX_PINCM55)
/* Defines for R4: GPIOA.27 with pinCMx 60 on package pin 31 */
#define GPIO_XUNJI_R4_PORT                                               (GPIOA)
#define GPIO_XUNJI_R4_PIN                                       (DL_GPIO_PIN_27)
#define GPIO_XUNJI_R4_IOMUX                                      (IOMUX_PINCM60)
/* Port definition for Pin Group ENCODER */
#define ENCODER_PORT                                                     (GPIOB)

/* Defines for Left_A: GPIOB.5 with pinCMx 18 on package pin 53 */
// pins affected by this interrupt request:["Left_A","Left_B","Right_A","Right_B"]
#define ENCODER_INT_IRQN                                        (GPIOB_INT_IRQn)
#define ENCODER_INT_IIDX                        (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define ENCODER_Left_A_IIDX                                  (DL_GPIO_IIDX_DIO5)
#define ENCODER_Left_A_PIN                                       (DL_GPIO_PIN_5)
#define ENCODER_Left_A_IOMUX                                     (IOMUX_PINCM18)
/* Defines for Left_B: GPIOB.4 with pinCMx 17 on package pin 52 */
#define ENCODER_Left_B_IIDX                                  (DL_GPIO_IIDX_DIO4)
#define ENCODER_Left_B_PIN                                       (DL_GPIO_PIN_4)
#define ENCODER_Left_B_IOMUX                                     (IOMUX_PINCM17)
/* Defines for Right_A: GPIOB.11 with pinCMx 28 on package pin 63 */
#define ENCODER_Right_A_IIDX                                (DL_GPIO_IIDX_DIO11)
#define ENCODER_Right_A_PIN                                     (DL_GPIO_PIN_11)
#define ENCODER_Right_A_IOMUX                                    (IOMUX_PINCM28)
/* Defines for Right_B: GPIOB.12 with pinCMx 29 on package pin 64 */
#define ENCODER_Right_B_IIDX                                (DL_GPIO_IIDX_DIO12)
#define ENCODER_Right_B_PIN                                     (DL_GPIO_PIN_12)
#define ENCODER_Right_B_IOMUX                                    (IOMUX_PINCM29)
/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOA)

/* Defines for SCL1: GPIOA.1 with pinCMx 2 on package pin 34 */
#define OLED_SCL1_PIN                                            (DL_GPIO_PIN_1)
#define OLED_SCL1_IOMUX                                           (IOMUX_PINCM2)
/* Defines for SDA1: GPIOA.0 with pinCMx 1 on package pin 33 */
#define OLED_SDA1_PIN                                            (DL_GPIO_PIN_0)
#define OLED_SDA1_IOMUX                                           (IOMUX_PINCM1)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWMAB_init(void);
void SYSCFG_DL_TIMER_A1_KONG_ZHI_init(void);
void SYSCFG_DL_UART_1_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

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
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
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


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_LED */
#define PWM_LED_INST                                                       TIMG8
#define PWM_LED_INST_IRQHandler                                 TIMG8_IRQHandler
#define PWM_LED_INST_INT_IRQN                                   (TIMG8_INT_IRQn)
#define PWM_LED_INST_CLK_FREQ                                            5000000
/* GPIO defines for channel 1 */
#define GPIO_PWM_LED_C1_PORT                                               GPIOB
#define GPIO_PWM_LED_C1_PIN                                       DL_GPIO_PIN_22
#define GPIO_PWM_LED_C1_IOMUX                                    (IOMUX_PINCM50)
#define GPIO_PWM_LED_C1_IOMUX_FUNC                   IOMUX_PINCM50_PF_TIMG8_CCP1
#define GPIO_PWM_LED_C1_IDX                                  DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                         (49999U)



/* Defines for UART_Emm */
#define UART_Emm_INST                                                      UART0
#define UART_Emm_INST_FREQUENCY                                          4000000
#define UART_Emm_INST_IRQHandler                                UART0_IRQHandler
#define UART_Emm_INST_INT_IRQN                                    UART0_INT_IRQn
#define GPIO_UART_Emm_RX_PORT                                              GPIOA
#define GPIO_UART_Emm_TX_PORT                                              GPIOA
#define GPIO_UART_Emm_RX_PIN                                       DL_GPIO_PIN_1
#define GPIO_UART_Emm_TX_PIN                                       DL_GPIO_PIN_0
#define GPIO_UART_Emm_IOMUX_RX                                    (IOMUX_PINCM2)
#define GPIO_UART_Emm_IOMUX_TX                                    (IOMUX_PINCM1)
#define GPIO_UART_Emm_IOMUX_RX_FUNC                     IOMUX_PINCM2_PF_UART0_RX
#define GPIO_UART_Emm_IOMUX_TX_FUNC                     IOMUX_PINCM1_PF_UART0_TX
#define UART_Emm_BAUD_RATE                                              (115200)
#define UART_Emm_IBRD_4_MHZ_115200_BAUD                                      (2)
#define UART_Emm_FBRD_4_MHZ_115200_BAUD                                     (11)




/* Defines for SPI_LCD */
#define SPI_LCD_INST                                                       SPI1
#define SPI_LCD_INST_IRQHandler                                 SPI1_IRQHandler
#define SPI_LCD_INST_INT_IRQN                                     SPI1_INT_IRQn
#define GPIO_SPI_LCD_PICO_PORT                                            GPIOB
#define GPIO_SPI_LCD_PICO_PIN                                     DL_GPIO_PIN_8
#define GPIO_SPI_LCD_IOMUX_PICO                                 (IOMUX_PINCM25)
#define GPIO_SPI_LCD_IOMUX_PICO_FUNC                 IOMUX_PINCM25_PF_SPI1_PICO
/* GPIO configuration for SPI_LCD */
#define GPIO_SPI_LCD_SCLK_PORT                                            GPIOB
#define GPIO_SPI_LCD_SCLK_PIN                                     DL_GPIO_PIN_9
#define GPIO_SPI_LCD_IOMUX_SCLK                                 (IOMUX_PINCM26)
#define GPIO_SPI_LCD_IOMUX_SCLK_FUNC                 IOMUX_PINCM26_PF_SPI1_SCLK



/* Port definition for Pin Group GPIO_LCD */
#define GPIO_LCD_PORT                                                    (GPIOB)

/* Defines for PIN_RES: GPIOB.10 with pinCMx 27 on package pin 62 */
#define GPIO_LCD_PIN_RES_PIN                                    (DL_GPIO_PIN_10)
#define GPIO_LCD_PIN_RES_IOMUX                                   (IOMUX_PINCM27)
/* Defines for PIN_DC: GPIOB.11 with pinCMx 28 on package pin 63 */
#define GPIO_LCD_PIN_DC_PIN                                     (DL_GPIO_PIN_11)
#define GPIO_LCD_PIN_DC_IOMUX                                    (IOMUX_PINCM28)
/* Defines for PIN_CS: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_LCD_PIN_CS_PIN                                     (DL_GPIO_PIN_14)
#define GPIO_LCD_PIN_CS_IOMUX                                    (IOMUX_PINCM31)
/* Defines for PIN_BLK: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_LCD_PIN_BLK_PIN                                    (DL_GPIO_PIN_26)
#define GPIO_LCD_PIN_BLK_IOMUX                                   (IOMUX_PINCM57)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_LED_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_UART_Emm_init(void);
void SYSCFG_DL_SPI_LCD_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

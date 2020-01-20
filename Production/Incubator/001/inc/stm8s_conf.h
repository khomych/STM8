/**
  ******************************************************************************
  * @file    stm8s_conf.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file is used to configure the Library.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_CONF_H
#define __STM8S_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"


//Modified for STM8S103F3


#include "stm8s_adc1.h" 

//#include "stm8s_awu.h"
//#include "stm8s_beep.h"

#include "stm8s_clk.h"
//#include "stm8s_exti.h"
//#include "stm8s_flash.h"
#include "stm8s_gpio.h"
#include "stm8s_i2c.h"
//#include "stm8s_itc.h"
//#include "stm8s_iwdg.h"
//#include "stm8s_rst.h"
//#include "stm8s_spi.h"
//#include "stm8s_tim1.h"

//#include "stm8s_tim2.h"

#include "stm8s_tim4.h"

#include "stm8s_uart1.h"

//#include "stm8s_wwdg.h"



/* Uncomment the line below to create your own interrupt handlers */
//#defineTRAP_IRQ                   /* TRAP */

//#define TLI_IRQ                   /* TLI */

//#define AWU_IRQ                   /* AWU */

//#define CLK_IRQ                   /* CLOCK */

//#define EXTI_PORTA_IRQ            /* EXTI PORTA */
//#define EXTI_PORTB_IRQ            /* EXTI PORTB */
//#define EXTI_PORTC_IRQ            /* EXTI PORTC */
//#define EXTI_PORTD_IRQ            /* EXTI PORTD */
//#define EXTI_PORTE_IRQ            /* EXTI PORTE */

//#define TIM1_CAP_COM_IRQ          /* TIM1 CAP/COM */
//#define TIM1_UPD_OVF_TRG_BRK_IRQ  /* TIM1 UPD/OVF/TRG/BRK */
//#define TIM2_UPD_OVF_BRK_IRQ      /* TIM2 UPD/OVF/BRK */
//#define TIM2_CAP_COM_IRQ          /* TIM2 CAP/COM */
//#define TIM4_UPD_OVF_IRQ          /* TIM4 UPD/OVF */

//#define UART1_TX_IRQ              /* UART1 TX */
//#define UART1_RX_IRQ              /* UART1 RX */

//#define SPI_IRQ                   /* SPI */

//#define I2C_IRQ                   /* I2C */

//#define ADC1_IRQ                  /* ADC1 */

//#define EEPROM_EEC_IRQ            /* EEPROM ECC CORRECTION */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define assert_param(expr) ((void)0)


#endif /* __STM8S_CONF_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**

  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#include "stm8s_conf.h"

/* I/Os configuration */

#define LED_PORT        GPIOB
#define LED_PIN         GPIO_PIN_5

#define LED_ON        GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_OFF       GPIO_WriteHigh(LED_PORT, LED_PIN)

#ifndef LCD_SCK_GPIO_Port
#define LCD_SCK_GPIO_Port	GPIOA		//enter
#endif

#ifndef LCD_SCK_Pin
#define LCD_SCK_Pin		GPIO_PIN_1	//enter
#endif

#ifndef LCD_STROBE_GPIO_Port
#define LCD_STROBE_GPIO_Port	GPIOA		//enter
#endif

#ifndef LCD_STROBE_Pin
#define LCD_STROBE_Pin		GPIO_PIN_3	//enter
#endif

#ifndef LCD_DATA_GPIO_Port
#define LCD_DATA_GPIO_Port	GPIOA			//enter
#endif

#ifndef LCD_DATA_Pin
#define LCD_DATA_Pin		GPIO_PIN_2	//enter
#endif



#define KEY_ENCODER_PORT  GPIOC
#define KEY_ENCODER_PIN   GPIO_PIN_5

    
     
     
     
     
     
     
     
     
/* Private function prototypes -----------------------------------------------*/
  
 
void KeyScan(void);
void EncoderScan(void);
void KeyEncoderAction(void);

void SysClock_Cong(void);
void UART1_Conf(void);

void GPIO_Conf(void);
void TIM1_Conf();
void TIM4_Conf(void);
void EXTI_Conf(void);

void action_IRQ_EXTI(void);

void actionTimer_1S(void);
/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#endif /* __MAIN_H */


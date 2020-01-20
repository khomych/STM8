/**

  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* I/Os configuration */

#define LED_PORT        GPIOB
#define LED_PIN         GPIO_PIN_5

#define LED_ON        GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_OFF       GPIO_WriteHigh(LED_PORT, LED_PIN)

      
    
/*
#define A70_RST_PIN GPIO_PIN_3
#define A70_RST_PORT GPIOC

#define A70_DC_PIN GPIO_PIN_4
#define A70_DC_PORT GPIOC

#define A70_SCE_PIN GPIO_PIN_4
#define A70_SCE_PORT GPIOD

#define A70_RST_L		A70_RST_PORT->ODR &= ~A70_RST_PIN 	
#define A70_RST_H		A70_RST_PORT->ODR |= A70_RST_PIN 

#define A70_DC_L		A70_DC_PORT->ODR &= ~A70_DC_PIN	
#define A70_DC_H		A70_DC_PORT->ODR |= A70_DC_PIN

#define A70_SCE_L		A70_SCE_PORT->ODR &= ~A70_SCE_PIN	
#define A70_SCE_H		A70_SCE_PORT->ODR |= A70_SCE_PIN
*/
#define TEST_PIN GPIO_PIN_5
#define TEST_PORT GPIOD

#define TEST_L		TEST_PORT->ODR &= ~TEST_PIN	
#define TEST_H		TEST_PORT->ODR |= TEST_PIN
     
     
     
/* Private function prototypes -----------------------------------------------*/
  

void SysClock_Cong(void);
void UART1_Conf(void);
void SPI_Conf(void);
void GPIO_Conf(void);
void TIM4_Conf(void);
void EXTI_Conf(void);

void action_IRQ_EXTI(void);

void actionTimer_1S(void);
/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#endif /* __MAIN_H */


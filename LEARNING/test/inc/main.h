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

#define SCK_PIN         GPIO_PIN_2
#define SCK_PORT        GPIOA
#define CS_PIN          GPIO_PIN_1
#define CS_PORT         GPIOA
#define DO_PIN          GPIO_PIN_6
#define DO_PORT         GPIOD
       

// Chip Enable Activates RX or TX mode
#define SCK_L()         GPIO_WriteLow(SCK_PORT, SCK_PIN)
#define SCK_H()         GPIO_WriteHigh(SCK_PORT, SCK_PIN)

// SPI Chip Select
#define CS_L()        GPIO_WriteLow(CS_PORT, CS_PIN)
#define CS_H()        GPIO_WriteHigh(CS_PORT, CS_PIN)

    
     
     
     
     
     
     
     
     
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


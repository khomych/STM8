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


       

     
     
/* Private function prototypes -----------------------------------------------*/
  
void Led(uint8_t status);

void SysClock_Cong(void);
void UART1_Conf(void);
void SPI_Conf(void);
void GPIO_Conf(void);
void TIM2_Conf(void);
void TIM4_Conf(void);
void EXTI_Conf(void);

void action_IRQ_EXTI(void);

void actionTimer_10mS(void);
void actionTimer_100mS(void);
void actionTimer_1S(void);


int16_t Get_Reference(void);

int16_t Get_Measurement(void);

/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#endif /* __MAIN_H */


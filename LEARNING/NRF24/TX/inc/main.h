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
C7 - MISO
C6 - MOSI
C5 - SCK
C4 - CE
C3 - SCN
B4 - IRQ
    */
    
    
    
#define CE_PIN          GPIO_PIN_4
#define CE_GPIO_PORT    GPIOC
#define CS_PIN         GPIO_PIN_3
#define CS_GPIO_PORT   GPIOC
       
#define IRQ_PIN         GPIO_PIN_2
#define IRQ_GPIO_PORT   GPIOD    
#define IRQ_EXTI_PORT   EXTI_PORT_GPIOD

#define OUT_PIN         GPIO_PIN_3
#define OUT_GPIO_PORT   GPIOD

// OUT
#define OUT_L()        GPIO_WriteLow(OUT_GPIO_PORT, OUT_PIN)
#define OUT_H()        GPIO_WriteHigh(OUT_GPIO_PORT, OUT_PIN)


// Chip Enable Activates RX or TX mode
#define nRF24_CE_L()         GPIO_WriteLow(CE_GPIO_PORT, CE_PIN)
#define nRF24_CE_H()         GPIO_WriteHigh(CE_GPIO_PORT, CE_PIN)

// SPI Chip Select
#define nRF24_CSN_L()        GPIO_WriteLow(CS_GPIO_PORT, CS_PIN)
#define nRF24_CSN_H()        GPIO_WriteHigh(CS_GPIO_PORT, CS_PIN)

    
     
     
     
     
     
     
     
     
/* Private function prototypes -----------------------------------------------*/
  
void Led(uint8_t status);
      
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


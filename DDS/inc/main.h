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




#define CE_PIN          GPIO_PIN_4
#define CE_GPIO_PORT    GPIOC

#define DC_PIN          GPIO_PIN_3
#define DC_GPIO_PORT    GPIOC
       
#define DATA_PIN         GPIO_PIN_2
#define DATA_GPIO_PORT   GPIOD    

#define CLK_PIN         GPIO_PIN_2
#define CLK_GPIO_PORT   GPIOD    



// Macros for CE Pin
#define DSP_CE_L()         GPIO_WriteLow(CE_GPIO_PORT, CE_PIN)
#define DSP_CE_H()         GPIO_WriteHigh(CE_GPIO_PORT, CE_PIN)

// Macros for DC Pin
#define DSP_DC_L()         GPIO_WriteLow(DC_GPIO_PORT, DC_PIN)
#define DSP_DC_H()         GPIO_WriteHigh(DC_GPIO_PORT, DC_PIN)

// Macros for DATA Pin
#define DSP_DATA_L()       GPIO_WriteLow(DATA_GPIO_PORT, DATA_PIN)
#define DSP_DATA_H()       GPIO_WriteHigh(DATA_GPIO_PORT, DATA_PIN)

// Macros for CLK Pin
#define DSP_CLK_L()        GPIO_WriteLow(CLK_GPIO_PORT, CLK_PIN)
#define DSP_CLK_H()        GPIO_WriteHigh(CLK_GPIO_PORT, CLK_PIN)
     
     
     
     
     
     
     
     
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


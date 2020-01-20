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

//Кнопка +
#define KEY_P_PORT GPIOD
#define KEY_P_PIN GPIO_PIN_1

//Кнопка -
#define KEY_M_PORT GPIOC
#define KEY_M_PIN GPIO_PIN_7
     
//Переключатель Режим
#define KEY_MODE_PORT GPIOD
#define KEY_MODE_PIN GPIO_PIN_2

//Контроль питания
#define POW_CTRL_PORT GPIOD
#define POW_CTRL_PIN GPIO_PIN_6

//Управление помпой
#define POMP_PORT GPIOD
#define POMP_PIN GPIO_PIN_5
     
     
/* Private function prototypes -----------------------------------------------*/
  
void Beep(uint8_t status);
    
void SysClock_Cong(void);
static void IWDG_Config(void);
void SPI_Conf(void);
void GPIO_Conf(void);
void TIM4_Conf(void);

void actionTimer_1S(void);
/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#endif /* __MAIN_H */


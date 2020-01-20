#include "stm8s_conf.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

#include "lib_pwm.h"


/**
  *inc->stm8s_conf.h - раскоментировать строчки с нужными хедерами 
  *периферии и необходимыми обработчиками прерываний. Все закоментированные
  *обработчики ведут на бесконечные циклы.
  * 
  *Project->Options->General Options - выбрать мк
  *
  *Project->Options->Debugger - выбрать отладчик
  *
  *Project->Options->C/C++ Compiler->Preprocessor->Defined symbols  - задать
  *семейство процессора(перечислены в lib->SPL->inc->stm8s.h), а также задать
  *частоты внутренних и внешних генераторов(если не задать, то будут ипользованы
  *значения по умолчанию из stm8s.h).
  */

/* Available ports on STM8S103P3:
 *
 * pin  1 (D4) PWM channel 1
 * pin 20 (D3) PWM channel 2
 * pin 10 (A3) PWM channel 3
 *
 */

/*
Подключение периферии:
VCC - Красный
GND - Синий
PD6 - Power Control ##GPIO IN## Розовый
PD5 - Pomp ##GPIO OUT## Зеленый
PD4 - BEEPER 
PD3 - SERVO Оранжевый
PD2 - Button Mode (Manual - 0, Auto - 1) ##GPIO IN##
PD1 - Button Plus ##GPIO IN##
PC7 - Button Minus ##GPIO IN##
PC6 - MOSI Display
PC5 - SCK Display
PC4 - CS Display ##GPIO OUT##
PC3 - Reserved Коричневый
PA1 - Temp Sensor 1 Желтый
PA2 - Temp Sensor 2 Белый



*/



#define LED_DELAY 50
#define BEEP_DELAY 40

//                      BEEP_FREQUENCY_1KHZ
//                      BEEP_FREQUENCY_2KHZ
//                      BEEP_FREQUENCY_4KHZ

#define BEEP_FREQUENCY  BEEP_FREQUENCY_1KHZ

 

uint8_t u8Pwm = 0;


bool b_1S_Flag = FALSE; //Флаг события 1 секунда
bool b_EXTI_Flag = FALSE; //Флаг события внешнего прерывания
uint32_t u32Millis; //Счетчик миллисекунд

uint32_t u32LedCounter = 0;
uint32_t u32BeepCounter = 0;
bool bBeepStatus = FALSE;


void Led(uint8_t status)
{
  if(status == 1)
  {
    u32LedCounter = u32Millis + LED_DELAY;
    LED_ON;
  }
  else
  {
    if(u32LedCounter > u32Millis)
    {

      LED_ON;
    }
    else
    {
      u32LedCounter = 0;
      LED_OFF;
    }
  }
}

void Beep(uint8_t status)
{
  if(status == 1)
  {
    if(bBeepStatus == FALSE)
    {
      BEEP_Cmd(ENABLE);
      bBeepStatus = TRUE;
    }
    u32BeepCounter = u32Millis + BEEP_DELAY;
    
    
  }
  else
  {
    if(u32BeepCounter > u32Millis)
    {
      
    if(bBeepStatus == FALSE)
    {
      BEEP_Cmd(ENABLE);
      bBeepStatus = TRUE;
    }
    
    }
    
    else
    {
      u32BeepCounter = 0;
      BEEP_Cmd(DISABLE);
      bBeepStatus = FALSE;
    }
  }
}

void main(void)
{
  
  SysClock_Cong();
  TIM4_Conf();
  //GPIO_Conf();

  SPI_Conf();

  
  pwm_init(PWM_SERVO, PWM_C2 );
  pwm_servo(PWM_C2, 0);
  
  BEEP_Init(BEEP_FREQUENCY);
  
    
  enableInterrupts();

  

  while (1)
  {
    
    //Led(0);
    Beep(0);
    if(b_1S_Flag == TRUE) actionTimer_1S();

  };
}



/*****************************************************************************
*
*   Имя функции : actionTimer_1S
*
*   Возвращаемые значения :
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionTimer_1S(void)
{
pwm_servo(PWM_C2, u8Pwm);
u8Pwm = u8Pwm + 10;
//Led(1); 
Beep(1);
b_1S_Flag = FALSE;
}



/*****************************************************************************
*
*   Имя функции : SysClock_Cong
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void SysClock_Cong(void)
{
       // ---------- CLK CONFIG -----------------------
    CLK_DeInit();

    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1); // set 16 MHz for CPU
    
    

    // uncomment if use HSE on Quartz
    //status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

}



/*****************************************************************************
*
*   Имя функции : TIM4_Conf
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void TIM4_Conf()
{
   #define TIM4_PERIOD       124     //1ms
   
   TIM4_Cmd(DISABLE);       // stop
   TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
   TIM4_ClearFlag(TIM4_FLAG_UPDATE);
   TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
   TIM4_Cmd(ENABLE);       // let's go
}
/*****************************************************************************
*
*   Имя функции : GPIO_Conf
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void GPIO_Conf()
{
   GPIO_DeInit(GPIOB);
   GPIO_DeInit(GPIOC);
   GPIO_DeInit(GPIOD);

   GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);       // LED PIN INIT
   

   
    
  
  
}

/*****************************************************************************
*
*   Имя функции : SPI_Conf
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void SPI_Conf()
{
    /* Enable SPI clock */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

  /* Set the MOSI and SCK at high level */
  //GPIO_ExternalPullUpConfig(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_6 |GPIO_PIN_5), ENABLE);

  
    //SPI
  
  SPI_Init(
      SPI_FIRSTBIT_MSB,
      SPI_BAUDRATEPRESCALER_256,
      SPI_MODE_MASTER,
      SPI_CLOCKPOLARITY_LOW,
      SPI_CLOCKPHASE_1EDGE,
      SPI_DATADIRECTION_2LINES_FULLDUPLEX,
      SPI_NSS_SOFT,
      (uint8_t)0x07
  );
 
  
  /*SPI_Init( SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_1LINE_TX,
           SPI_NSS_SOFT, 0x07);
*/

  /* SD_SPI enable */
  SPI_Cmd( ENABLE);


}







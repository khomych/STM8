#include "stm8s_conf.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

#include "lib_1wire.h"
#include "lib_ds18b20.h"

#include "Disp_7seg_555.h"

//#include "stdint.h"
#include "pid.h"


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

#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
    



//! \xrefitem todo "Todo" "Todo list"
#define K_P     20
//! \xrefitem todo "Todo" "Todo list"
#define K_I     0.1
//! \xrefitem todo "Todo" "Todo list"
#define K_D     0.5


bool b_10mS_Flag; 
bool b_100mS_Flag;
bool b_Key_Scan_Flag;

bool b_1S_Flag = FALSE; //Флаг события 1 секунда
bool b_EXTI_Flag = FALSE; //Флаг события внешнего прерывания
uint32_t u32Millis; //Счетчик миллисекунд

uint32_t u32LedCounter = 0;

extern uint16_t u16DispBuffer;;
extern uint8_t u8DispBuffDigit[3];
extern uint8_t CharSet[16];

uint8_t tmp = 0;

//Структуры с данными термодатчиков
OW_Device owdTerm;

//! Parameters for regulator
struct PID_DATA pidData;

uint16_t CCR3_Val = 0;

int16_t referenceValue, measurementValue, inputValue;
/*! \brief Read reference value.
 *
 * This function must return the reference value.
 * May be constant or varying
 */
int16_t Get_Reference(void)
{
  return 260;
}

/*! \brief Read system process value
 *
 * This function must return the measured data
 */
int16_t Get_Measurement(void)
{
  return owdTerm.Temperature10;;
}


//*****************************************************************************
//
//*****************************************************************************

void Led(uint8_t status)
{
  if(status == 1)
  {
    u32LedCounter = u32Millis + 50;
    LED_ON;
  }
  else
  {
    if(u32LedCounter > u32Millis)
    {
      //u32LedCounter--;
      LED_ON;
    }
    else
    {
      u32LedCounter = 0;
      LED_OFF;
    }
  }
}



void main(void)
{
  
  SysClock_Cong();
  TIM2_Conf();
  TIM4_Conf();
  GPIO_Conf();
  UART1_Conf();
  
  DisplayInit();
  
  
  //1w init
  owdTerm.GPIOx = THERM0_PORT;
  owdTerm.GPIO_Pin = THERM0_PIN;

  //enableInterrupts();
  pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidData);
  
  
  DS18B20_Init(&owdTerm);
      
  Ds18B20_Mode(&owdTerm, THERM_MODE_12BIT);
  DS18B20_StartConvTemp(&owdTerm);

  
  printf("\n\rSystem Init\n\r");
  
  enableInterrupts();
  printf("\n\rEnable Interrupts\n\r");
  
  DisplaySetDots(0x02);

  
  while (1)
  {
    
    Led(0);
    if(b_10mS_Flag == TRUE)actionTimer_10mS();
    if(b_100mS_Flag == TRUE)actionTimer_100mS();
    if(b_1S_Flag == TRUE) actionTimer_1S();

  //printf("df");
  };
}

/*****************************************************************************
*
*   Имя функции : action_IRQ_EXTI
*
*   Возвращаемые значения :
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void action_IRQ_EXTI(void)
{

}


/*****************************************************************************
*
*   Имя функции : actionTimer_100mS
*
*   Возвращаемые значения :
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionTimer_10mS(void)
{
  
  
  referenceValue = Get_Reference();
  measurementValue = Get_Measurement();
  
  inputValue = pid_Controller(referenceValue, measurementValue, &pidData) / 128;



  b_10mS_Flag = FALSE;
}

/*****************************************************************************
*
*   Имя функции : actionTimer_100mS
*
*   Возвращаемые значения :
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionTimer_100mS(void)
{
  
  CCR3_Val = CCR3_Val + 50;
  if(CCR3_Val > 999)CCR3_Val = 0;
  
    /* Set the Pulse value */
  TIM2->CCR3H = (uint8_t)(CCR3_Val >> 8);
  TIM2->CCR3L = (uint8_t)(CCR3_Val);
  

  printf("%d => %d\n\r",(int)measurementValue, (int)inputValue);


  b_100mS_Flag = FALSE;
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
  
int16_t i16Temperature;
uint8_t tmpbuff[3];
DS18B20_GetTempToInt10(&owdTerm);
i16Temperature = owdTerm.Temperature10;
DS18B20_StartConvTemp(&owdTerm);

tmpbuff[0] = i16Temperature / 100;
tmpbuff[1] = (i16Temperature - tmpbuff[0] * 100) / 10;
tmpbuff[2] =  i16Temperature - tmpbuff[0] * 100 - tmpbuff[1] * 10;

u8DispBuffDigit[0] = CharSet[tmpbuff[0]];
u8DispBuffDigit[1] = CharSet[tmpbuff[1]];
u8DispBuffDigit[2] = CharSet[tmpbuff[2]];

//Led(1);
DiaplayCalculate();




   
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
*   Имя функции : UART1_Conf
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача : Иницализация UART1
*            
*            
*****************************************************************************/
void UART1_Conf(void)
{
  UART1_DeInit();
  /* UART1 configuration ------------------------------------------------------*/
  /* UART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
        - UART1 Clock disabled
  */
  UART1_Init(
                (uint32_t)115200,
                UART1_WORDLENGTH_8D,
                UART1_STOPBITS_1,
                UART1_PARITY_NO,
                UART1_SYNCMODE_CLOCK_DISABLE,
                UART1_MODE_TXRX_ENABLE
             ); 
}

/*****************************************************************************
*
*   Имя функции : TIM2_Conf
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*       
*/     
void TIM2_Conf(void)
{
    /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, 999);

  /* PWM1 Mode configuration: Channel1 */ 
  //TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,CCR1_Val, TIM2_OCPOLARITY_HIGH);
  //TIM2_OC1PreloadConfig(ENABLE);

  /* PWM1 Mode configuration: Channel2 */ 
  //TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,CCR2_Val, TIM2_OCPOLARITY_HIGH);
  //TIM2_OC2PreloadConfig(ENABLE);

  /* PWM1 Mode configuration: Channel3 */         
  TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,CCR3_Val, TIM2_OCPOLARITY_HIGH);
  TIM2_OC3PreloadConfig(ENABLE);

  TIM2_ARRPreloadConfig(ENABLE);

  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
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





/**
  * @brief Retargets the C library printf function to the UART.
  * @param c Character to send
  * @retval char Character sent
  */
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  UART1_SendData8(c);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);

  return (c);
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval char Character to Read
  */
GETCHAR_PROTOTYPE
{

  int c = 0;
  /* Loop until the Read data register flag is SET */
  while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
    c = UART1_ReceiveData8();
  return (c);
}



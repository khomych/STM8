#include "stm8s_conf.h"
#include "main.h"
//#include "stdio.h"
#include "string.h"

#include "lcd_0802_595.h"
#include "function.h"


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



    
uint8_t tmp = 0;

#define KEY_NOIZE_TIME 255;


bool b_Key_Encoder_Old = TRUE;
uint16_t u16Old_Encoder;

uint8_t u8Key_Encoder_NoizeTrim = 0;
 
bool b_100mS_Flag = FALSE; //Флаг события 100ms
bool b_Key_Scan_Flag = FALSE; //Флаг события KeyScan
bool b_Encoder_Scan_Flag = FALSE; //Флаг события EncoderScan


bool b_1S_Flag = FALSE; //Флаг события 1 секунда
bool b_EXTI_Flag = FALSE; //Флаг события внешнего прерывания
uint32_t u32Millis; //Счетчик миллисекунд



extern bool bPacketReceive;

//******************************************************************************
//                              KeyScan
//******************************************************************************
void KeyScan(void)
{
  //Key ENCODER
  if((KEY_ENCODER_PORT->IDR & (uint8_t)KEY_ENCODER_PIN) == 0)
  {
    if((u8Key_Encoder_NoizeTrim == 0) && (b_Key_Encoder_Old == TRUE))
    {
      u8Key_Encoder_NoizeTrim = KEY_NOIZE_TIME;
      b_Key_Encoder_Old = FALSE;
      //Press Action!!!
      KeyEncoderAction();
    }
  }
  
  else 
  {
    if(b_Key_Encoder_Old == FALSE)
    {
      b_Key_Encoder_Old = TRUE;
    }
  }
 
  if(u8Key_Encoder_NoizeTrim > 0)u8Key_Encoder_NoizeTrim--;
  
  b_Key_Scan_Flag = FALSE;
}

void EncoderScan(void)
{

  uint16_t u16CurrentEncoder = TIM1_GetCounter() / 8;
  if(u16CurrentEncoder != u16Old_Encoder){
    Beep(BEEP_ONE);
    LCD_GotoXY(0, 0);
    LCD_Char(0x30 + u16CurrentEncoder);
    u16Old_Encoder = u16CurrentEncoder;
  }
    b_Encoder_Scan_Flag = FALSE;
}

void KeyEncoderAction(void)
{
  Beep(BEEP_ONE);
  LCD_GotoXY(0, 1);
  LCD_Char('X');
}

void main(void)
{
  
  SysClock_Cong();
  TIM1_Conf();
  TIM4_Conf();
  GPIO_Conf();
  EXTI_Conf();

  UART1_Conf();
  BEEP_Init(BEEP_FREQUENCY);
  
  //Включаем прерывание от приема данных.
  UART1_ITConfig( UART1_IT_RXNE_OR, ENABLE);

  
  enableInterrupts();

  
  LCD_Init(0);
  LCD_Clear();
  LCD_Light(LCD_BL_HI);
  LCD_GotoXY(0, 0);
        
        
  LCD_GotoXY(0, 0);
  LCD_Char(0x30 + 1);
  LCD_Char(0x30 + 2);
  LCD_Char(0x30 + 3);
  LCD_Char(',');
  LCD_Char(0x30 + 4);
  
 
  



  
  while (1)
  {
    Beep(BEEP_REFRESH);
    Led(LED_REFRESH);
    if(b_1S_Flag == TRUE) actionTimer_1S();
    if(b_Key_Scan_Flag == TRUE)KeyScan();
    if(b_Encoder_Scan_Flag ==TRUE)EncoderScan();
    
    if(bPacketReceive == TRUE){
      bPacketReceive = FALSE;
      ReceivePacket();
      Beep(BEEP_ONE);
      
    }
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

//SendPacket();

 //TIM1_GetCounter();
   
b_1S_Flag = FALSE;
}



/*****************************************************************************
*
*   Имя функции : EXTI_Conf
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void EXTI_Conf(void)
{
   
   //EXTI_DeInit();
   //EXTI_SetExtIntSensitivity(IRQ_EXTI_PORT, EXTI_SENSITIVITY_FALL_ONLY); 
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
*   Имя функции : TIM1_Conf
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void TIM1_Conf()
{
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 0xFFFF, 0);
  TIM1_EncoderInterfaceConfig(TIM1_ENCODERMODE_TI12, TIM1_ICPOLARITY_FALLING, TIM1_ICPOLARITY_FALLING);
  TIM1_Cmd(ENABLE);
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
   
   GPIO_Init(LCD_SCK_GPIO_Port, LCD_SCK_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);    //CE PIN
   GPIO_Init(LCD_STROBE_GPIO_Port, LCD_STROBE_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);    //CE PIN
   GPIO_Init(LCD_DATA_GPIO_Port, LCD_DATA_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);    //CE PIN

   GPIO_Init(KEY_ENCODER_PORT, KEY_ENCODER_PIN,GPIO_MODE_IN_PU_NO_IT);
   
   
   
    
  
  
}











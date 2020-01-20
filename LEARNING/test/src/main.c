#include "stm8s_conf.h"
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "i2c_drv.h"


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
    

  unsigned int tmp = 0;
uint16_t arrRes[5];

#define delay()   tmp=0x00FF; while(tmp--){}


 



bool b_1S_Flag = FALSE; //Флаг события 1 секунда
bool b_EXTI_Flag = FALSE; //Флаг события внешнего прерывания
uint32_t u32Millis; //Счетчик миллисекунд

uint16_t ReadTemp(void)
{
  uint16_t result = 0;
  
  CS_L();
  //delay();
  
  for(uint8_t i = 0; i < 16; i++)
  {
    result = result << 1;
    //SCK_H();
    SCK_PORT->ODR |= SCK_PIN;
    //delay();
    //if(GPIO_ReadInputPin(DO_PORT, DO_PIN))
    if(DO_PORT->IDR & DO_PIN)
    {
      result = result | 1;
    }
        
    //SCK_L();
    SCK_PORT->ODR &= ~SCK_PIN; 
    //delay();
  }
  
  
  
  
  CS_H();
  return result;
}




void main(void)
{
  
  SysClock_Cong();
  TIM4_Conf();
  GPIO_Conf();
  //EXTI_Conf();
  //SPI_Conf();
  UART1_Conf();
  
  //printf("\n\rSystem Init\n\r");
  
  

  //Инициализация I2C в режиме мастер
  //i2c_master_init(F_MASTER_HZ, F_I2C_HZ);
  
  //char * data = "0011223344556677";
  //uint8_t  buff[256];
  //uint8_t sbuf[20];
   
  //i2c_wr_reg(0xA0, 0, data, 16);
  //printf("wrOK\n\r");
     
  tmp=0x05FF;     //~11ms pause for write eeprom page (16 bytes for AT24C04)
  while(tmp--){}

  //i2c_rd_reg(0xA0, 0, buff, 255);


  //printf(buff);
  printf("\n\r");

  
  enableInterrupts();
  //printf("\n\rEnable Interrupts\n\r");
  
ReadTemp();
  while (1)
  {
    
  
  
  //eeReadByte(0xA0);
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
//eeReadByte(0xA0);
uint16_t res = ReadTemp();
res = res >> 3;

arrRes[4] = arrRes[3];
arrRes[3] = arrRes[2];
arrRes[2] = arrRes[1];
arrRes[1] = arrRes[0];
arrRes[0] = res;

res = (uint32_t)((arrRes[4] + arrRes[3] + arrRes[2] + arrRes[1] + arrRes[0]) / 5);

res = res / 4;
char buff[20];
sprintf(buff, "%u.%u C\n\r", res, (res - ((res / 4) * 4))*25 );

printf(buff);
   
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
    
      CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);

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
   
   
   GPIO_Init(SCK_PORT, SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);    //CE PIN
   GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);  //CS PIN
   GPIO_Init(DO_PORT, DO_PIN, GPIO_MODE_IN_PU_NO_IT);          //IRQ PIN
   
   GPIO_WriteLow(SCK_PORT, SCK_PIN);
   GPIO_WriteHigh(CS_PORT, CS_PIN);
    
  
  
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



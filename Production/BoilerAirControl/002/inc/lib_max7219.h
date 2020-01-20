/**
Модуль работает от 5В!
Для работы модуля MAX7219 необходимо настроить SPI. Рабочий пример:

void SPI_Conf()
{
  // Enable SPI clock 
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

  // SD_SPI Configuration 
  SPI_Init( SPI_FIRSTBIT_MSB,
            SPI_BAUDRATEPRESCALER_16,
            SPI_MODE_MASTER,
            SPI_CLOCKPOLARITY_LOW,
            SPI_CLOCKPHASE_1EDGE,
            SPI_DATADIRECTION_1LINE_TX,
            SPI_NSS_SOFT, 0x07);

  // SD_SPI enable 
  SPI_Cmd( ENABLE);
}



  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX7219_H
#define __MAX7219_H


/* Private define ------------------------------------------------------------*/
#define MAX7219_CS_PORT GPIOC
#define MAX7219_CS_PIN GPIO_PIN_4

#define MAX7219_CS_H  GPIO_WriteHigh(MAX7219_CS_PORT, MAX7219_CS_PIN)
#define MAX7219_CS_L  GPIO_WriteLow(MAX7219_CS_PORT, MAX7219_CS_PIN)    


/* Private variables -----------------------------------------------------------*/
 


/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void MAX7219_Shift(uint8_t u8Addr, uint8_t u8Data);
void MAX7219_Init(uint8_t u8Bbightness);
void MAX7219_SetBrightness(uint8_t u8Bbightness);
void MAX7219_Clear(void);




#endif /*MAX7219_H*/

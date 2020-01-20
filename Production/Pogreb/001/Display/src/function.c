
#include "stm8s_conf.h"
#include "main.h"

#include "function.h"







//*****************************************************************************
//              BEEPER VARIABLES
//****************************************************************************
uint32_t u32BeepCounter = 0;
bool bBeepStatus = FALSE;

extern uint32_t u32Millis; //Счетчик миллисекунд

MyBus DataReceive;
MyBus DataTransmite;
uint8_t u8TxCounter = 0;
uint8_t u8RxCounter = 0;

uint8_t GoodStartMarker[4] = {0xF1, 0xF2, 0xF3, 0xF4};

bool bPacketReceive = FALSE;


/*
  Name  : CRC-8
  Poly  : 0x31    x^8 + x^5 + x^4 + 1
  Init  : 0xFF
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
  MaxLen: 15 байт(127 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
uint8_t Crc8(uint8_t *pcBlock, uint8_t len)
{
    uint8_t crc = 0xFF;
    uint8_t i;

    while (len--)
    {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}



void SendPacket(void)
{
  
  DataTransmite.StartMarker = *((uint32_t *)(&GoodStartMarker));
  DataTransmite.TxAddr = DataReceive.RxAddr;
  DataTransmite.RxAddr = DataReceive.TxAddr;
  DataTransmite.Cmd = ReadRegister;
  DataTransmite.Status = DataReceive.Status;
  DataTransmite.Address = DataReceive.Address;
  DataTransmite.Data = DataReceive.Data;
  DataTransmite.CRC8 = DataReceive.CRC8;
  //Включаем прерывания если TX буфер свободен и начинаем отправлять данные.
UART1_ITConfig( UART1_IT_TXE, ENABLE);
}

void ReceivePacket(void)
{
  
  uint8_t crc = Crc8(((uint8_t *)(&DataReceive)) , DATA_PACKER_LEN-1);
  if(DataReceive.CRC8 != crc){
    DataReceive.Status = Error;
  }
  else {
    DataReceive.Status = OK;
  }
  SendPacket();
}


/*****************************************************************************
*
*   Имя функции : Millis
*
*   Возвращаемые значения : uint32_t количество миллисекунд, прошедших с начала запуска МК
*                           максимальное число 4294967296 мс
*                                              4294967 с
*                                              1193 часа 
*
*   Параметры : 
*
*   Задача : возвращает количество миллисекунд, прошедших с начала запуска МК
*            
*            
*****************************************************************************/
uint32_t Millis(void)
{
  return u32Millis;
}

/*****************************************************************************
*
*   Имя функции : Beep
*
*   Возвращаемые значения :
*
*   Параметры : (uint8_t)status. 0 - передавать в бесконечном цикле для обработки времени работы
*                                1 - одинарный Beep
*                                2 - двойной Beep
*
*   Задача : Beep
*            
*            
*****************************************************************************/
void Beep(uint8_t status)
{
  switch (status)
  {
  case 0:
    if(u32BeepCounter > Millis())
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
  break;
    
  case 1:
    if(bBeepStatus == FALSE)
    {
      BEEP_Cmd(ENABLE);
      bBeepStatus = TRUE;
    }
    u32BeepCounter = Millis() + BEEP_DELAY;  
  break;
    
  case 2:
    
    break;
    
    
  }
}

void Delay_ms(uint16_t u16Delay)
{
  uint32_t u32TimeOff = u32Millis + u16Delay;
  while (u32Millis < u32TimeOff)
  {
    nop();
  }
}
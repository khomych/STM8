
#include "stm8s_conf.h"
#include "main.h"

#include "function.h"

//время передачи 13-ти байт составляет 0.59 мс





//*****************************************************************************
//              BEEPER VARIABLES
//****************************************************************************
uint32_t u32BeepCounter = 0;
bool bBeepStatus = FALSE;
uint32_t u32LedCounter = 0;

extern uint32_t u32Millis; //Счетчик миллисекунд


MyPacket DataReceive;
MyPacket DataTransmite;

MyRegister Reg;
uint8_t u8TxCounter = 0;
uint8_t u8RxCounter = 0;

uint8_t GoodStartMarker[4] = {0xF1, 0xF2, 0xF3, 0xF4};
uint8_t MainAddress = 0x01;

bool bPacketReceive = FALSE;

uint8_t crc_array[256] =
{
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
    0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
    0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
    0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
    0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
    0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
    0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
    0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
    0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
    0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
    0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
    0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
    0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
    0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
    0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
    0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
    0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};


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



unsigned char DallasCrc8(const uint8_t * data, const uint8_t size)
{
    uint8_t crc = 0;
    for ( uint8_t i = 0; i < size; ++i )
    {
         crc = crc_array[data[i] ^ crc];
    }
    return crc;
}


void SendPacket(void)
{
  
  DataTransmite.Address = MainAddress;
  DataTransmite.Cmd = DataReceive.Cmd;
  DataTransmite.RegAddr = DataReceive.RegAddr;
  DataTransmite.RegData = DataReceive.RegData;
  DataTransmite.CRC8 = DallasCrc8(((uint8_t *)(&DataTransmite)) , DATA_PACKER_LEN-1);
  //Включаем прерывания если TX буфер свободен и начинаем отправлять данные.
  
UART1_ITConfig( UART1_IT_TXE, ENABLE);
}

void ReceivePacket(void)
{
  
  uint8_t crc = DallasCrc8(((uint8_t *)(&DataReceive)) , DATA_PACKER_LEN-1);
  if(DataReceive.CRC8 != crc){
    DataReceive.Cmd = ErrorCRC;
  }
  else {
    if(DataReceive.Address == 0 && DataReceive.Cmd == WriteRegister){
      //широковещательная передача - запись данных в регистр
      WriteReg(DataReceive.RegAddr, (int16_t *)(&DataReceive.RegData));
    }
    if(DataReceive.Address == MainAddress){
      //обращение к текущему устройству
      //возможны запрос статуса, запись и чтение регистра
      switch (DataReceive.Cmd){
        case WriteRegister:
          if(WriteReg(DataReceive.RegAddr, (int16_t *)(&DataReceive.RegData))){
            DataReceive.Cmd = StatusOK;
          }
          else {
            DataReceive.Cmd = ErrorRegister;
          }
          break;
          
      case ReadRegister:
        if(ReadReg(DataReceive.RegAddr, (int16_t *)(&DataReceive.RegData))){
          DataReceive.Cmd = WriteRegister;
        }
        else {
          DataReceive.Cmd = ErrorRegister;
        }
        break;
        
      case ReadStatus:
        DataReceive.Cmd = StatusOK;
        break;
        
      default:
        DataReceive.Cmd = ErrorCmd;
        break;
      }
    }
    SendPacket();
  }
}

/*****************************************************************************
*
*   Имя функции : ReadReg
*
*   Возвращаемые значения : int16_t значение регистра из памяти
*                           
*   Параметры : uint8_t address - адресс регистра
*
*   Задача : возвращает значение регистра
*            
*            
*****************************************************************************/
/*
typedef struct Register{
  int16_t TempIndoor;
  int16_t TempOutdoor;
  int16_t TempAdj;
  int8_t TempDelta;
  bool CoolerCold;
  bool CoolerHeater;
  bool CoolerCirk;
}MyRegister;
*/
bool ReadReg(uint16_t address, int16_t * data)
{
  if(address >= (uint16_t)REGISTER_COUNT)return FALSE;
  int16_t ret = 0x00;
  switch (address){
  case 0x0000:
    ret = Reg.TempIndoor;
  break;
    
  case 0x0001:
    ret = Reg.TempOutdoor;
  break;
  
  case 0x0002:
    ret = Reg.TempAdj;
  break;
  
  case 0x0003:
    ret = (int16_t)Reg.TempDelta;
  break;
  
  case 0x0004:
    ret = (int16_t)Reg.CoolerCold;
  break;
  
  case 0x0005:
    ret = (int16_t)Reg.CoolerHeater;
  break;
  
  case 0x0006:
    ret = Reg.CoolerCirk;
  break;
  
  default:
    return FALSE;
    break;
  }
  *data = ret;
  return TRUE;
}

/*****************************************************************************
*
*   Имя функции : WriteReg
*
*   Возвращаемые значения : нет

*
*   Параметры : uint8_t address - адресс регистра
*               int16_t data - значение регистра
*
*   Задача : запись данных в регистр
*            
*            
*****************************************************************************/
bool WriteReg(uint16_t address, int16_t * data)
{
 if(address >= REGISTER_COUNT)return FALSE;
  switch (address){
  case 0x0000:
    Reg.TempIndoor = *data;
  break;
    
  case 0x0001:
    Reg.TempOutdoor = *data;
  break;
  
  case 0x0002:
    Reg.TempAdj = *data;
  break;
  
  case 0x0003:
    Reg.TempDelta = *data;
  break;
  
  case 0x0004:
    Reg.CoolerCold = (bool)*data;
  break;
  
  case 0x0005:
    Reg.CoolerHeater = (bool)*data;
  break;
  
  case 0x0006:
    Reg.CoolerCirk = (bool)*data;
  break;
  
  default:
    return FALSE;
    break;
  }
  return TRUE;
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
*   Параметры : (uint8_t)status. BEEP_REFRESH - передавать в бесконечном цикле для обработки времени работы
*                                BEEP_ONE - одинарный Beep
*                                BEEP_DOUBLE - двойной Beep
*
*   Задача : Beep
*            
*            
*****************************************************************************/
void Beep(BeepCMD status)
{
  switch (status)
  {
  case BEEP_REFRESH:
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
    
  case BEEP_ONE:
    if(bBeepStatus == FALSE)
    {
      BEEP_Cmd(ENABLE);
      bBeepStatus = TRUE;
    }
    u32BeepCounter = Millis() + BEEP_DELAY;  
  break;
    
  case BEEP_DOUBLE:
    
    break;
    
    
  }
}

/*****************************************************************************
*
*   Имя функции : Led
*
*   Возвращаемые значения :
*
*   Параметры : (uint8_t)status. LED_REFRESH - передавать в бесконечном цикле для обработки времени работы
*                                LED_FLASH - одинарный 
*                                
*
*   Задача : Beep
*            
*            
*****************************************************************************/

void Led(LedCMD status)
{
  
  switch (status)
  {
  case LED_REFRESH:
    u32LedCounter = u32Millis + 50;
    LED_ON;
    break;
    
  case LED_FLASH:
    if(u32LedCounter > u32Millis){
      LED_ON;
    }
    else {
      u32LedCounter = 0;
      LED_OFF;
    }
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
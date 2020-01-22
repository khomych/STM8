/**

  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTION_H
#define __FUNCTION_H

//*****************************************************************************
//              BEEPER DEFINES
//*****************************************************************************
#define BEEP_DELAY 30

//                      BEEP_FREQUENCY_1KHZ
//                      BEEP_FREQUENCY_2KHZ
//                      BEEP_FREQUENCY_4KHZ

#define BEEP_FREQUENCY  BEEP_FREQUENCY_1KHZ

#define DATA_PACKER_LEN (uint8_t)7
#define REGISTER_COUNT 10

typedef enum {
  ReadStatus = (uint8_t)0,      //запрос статуса
  ReadRegister = (uint8_t)1,    //чтение регистра
  WriteRegister = (uint8_t)2,   //запись регистра
  ErrorCRC = (uint8_t)3,        //ошибка CRC8
  StatusOK = (uint8_t)4,        //ответ "стстус ОК"
  ErrorCmd = (uint8_t)5,         //неверная комманда  
  ErrorRegister = (uint8_t)6     //ошибка доступа к регистру
    
}MyBusCmd;


typedef enum {
  BEEP_REFRESH = (uint8_t)0x00,
  BEEP_ONE = (uint8_t)0x01,
  BEEP_DOUBLE = (uint8_t)0x02
}BeepCMD;

typedef enum {
  LED_REFRESH = (uint8_t)0x00,
  LED_FLASH = (uint8_t)0x01
}LedCMD;

typedef struct Packet {
  uint8_t Address;
  MyBusCmd Cmd;
  uint16_t RegAddr;
  int16_t RegData;
  uint8_t CRC8;
}MyPacket;

typedef struct Register{
  int16_t TempIndoor;
  int16_t TempOutdoor;
  int16_t TempAdj;
  int8_t TempDelta;
  bool CoolerCold;
  bool CoolerHeater;
  bool CoolerCirk;
}MyRegister;



uint32_t Millis(void);
void Beep(BeepCMD status);
void Delay_ms(uint16_t u16Delay);
void SendPacket(void);
void ReceivePacket(void);
void Led(LedCMD status);

bool ReadReg(uint16_t address, int16_t * data);
bool WriteReg(uint16_t address, int16_t * data);

uint8_t Crc8(uint8_t *pcBlock, uint8_t len);
unsigned char DallasCrc8(const uint8_t * data, const uint8_t size);




#endif /*__FUNCTION_H*/

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

#define DATA_PACKER_LEN (uint8_t)13

typedef enum {
  ReadRegister = (uint8_t)0,
  WriteRegister = (uint8_t)1
}MyBusCmd;

typedef enum {
  OK = (uint8_t)0x00,
  Error = (uint8_t)0x01
}MyBusStatus;

typedef enum {
  BEEP_REFRESH = (uint8_t)0x00,
  BEEP_ONE = (uint8_t)0x01,
  BEEP_DOUBLE = (uint8_t)0x02
}BeepCMD;

typedef enum {
  LED_REFRESH = (uint8_t)0x00,
  LED_FLASH = (uint8_t)0x01
}LedCMD;



typedef struct Bus{
  uint32_t StartMarker;
  uint8_t TxAddr;
  uint8_t RxAddr;
  MyBusCmd Cmd;
  MyBusStatus Status;
  uint16_t Address;
  int16_t Data;
  uint8_t CRC8;
}MyBus;



uint32_t Millis(void);
void Beep(BeepCMD status);
void Delay_ms(uint16_t u16Delay);
void SendPacket(void);
void ReceivePacket(void);
void Led(LedCMD status);

int16_t ReadReg(uint8_t address);
void WriteReg(uint8_t address, int16_t data);

uint8_t Crc8(uint8_t *pcBlock, uint8_t len);


#endif /*__FUNCTION_H*/

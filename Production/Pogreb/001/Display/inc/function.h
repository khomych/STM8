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

typedef enum {
  ReadRegister = (uint8_t)0,
  WriteRegister = (uint8_t)1
}MyBusCmd;

typedef enum {
  OK = (uint8_t)0x00,
  Error = (uint8_t)0x01
}MyBusStatus;

#define DATA_PACKER_LEN (uint8_t)13

typedef struct Bus{
  uint32_t StartMarker;
  uint8_t TxAddr;
  uint8_t RxAddr;
  MyBusCmd Cmd;
  MyBusStatus Status;
  uint16_t Address;
  uint16_t Data;
  uint8_t CRC8;
}MyBus;



uint32_t Millis(void);
void Beep(uint8_t status);
void Delay_ms(uint16_t u16Delay);
void SendPacket(void);
void ReceivePacket(void);

uint8_t Crc8(uint8_t *pcBlock, uint8_t len);


#endif /*__FUNCTION_H*/

/**

  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_DS18B20_H
#define __LIB_DS18B20_H

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#include "lib_1wire.h"

#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xBE
#define THERM_CMD_WSCRATCHPAD 0x4E
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xB8
#define THERM_CMD_RPWRSUPPLY 0xB4
#define THERM_CMD_SEARCHROM 0xF0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xCC
#define THERM_CMD_ALARMSEARCH 0xEC

//-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;
//--------------------------
// ВСЕ ПЗУ КОМАНДЫ DS18B20 |
//--------------------------

#define CONVERT_TEMP        0x44 //Команда к началу измерения температуры.
#define WRITE_SCRATCHPAD    0x4E //Записать пользовательские данные в регистры TEMP_TL, TEMP_TH и находящиеся в ПЗУ. 
#define READ_SCRATCHPAD     0xBE //Прочесть содержимое ОЗУ (9 байт).
#define COPY_SCRATCHPAD     0x48 //Сохранение текущих значений регистров TEMP_TL\TH в ПЗУ.
#define RECALL_E2           0xB8 //Команда действует обратным образом предыдущей.
#define READ_POWER_SUPPLY   0xB4 //Проверка используемого режима питания (обычное \ паразитное питание).

//-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;
 
#define THERM_DECIMAL_STEPS_12BIT 625 //.0625


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Режимы работы. По-умолчанию 12 бит.
typedef enum
{
  THERM_MODE_9BIT  = 0x1F,
  THERM_MODE_10BIT = 0x3F,
  THERM_MODE_11BIT = 0x5F,
  THERM_MODE_12BIT = 0x7F
} THERM_MODE;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!1      Заголовки функций       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
uint8_t Ds18B20_Mode(OW_Device * device, THERM_MODE mode);
void DS18B20_StartConvTemp(OW_Device * device);
void DS18B20_GetTempToInt10(OW_Device * device);
void CRC8(unsigned char x);

#endif /*__LIB_DS18B20_H*/

#ifndef MY_DS18B20_H
#define MY_ODS18B20_H

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "My_OneWire.h"


 

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
unsigned char Ds18B20_Mode(OW_Device * device, THERM_MODE mode);
void DS18B20_StartConvTemp(OW_Device * device);
void DS18B20_GetTempToInt10(OW_Device * device);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!1      Тела функций            !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/**
  * @brief Установить режим работы термодатчика
  * @param[in] mode Режим работы
  * @retval
  * Возвращаемое значение : 0 - оерация успешная; 1 - ошибка 
*/
unsigned char Ds18B20_Mode(OW_Device * device, THERM_MODE mode)
{
  unsigned char ucResult = 0;
  ucResult = OneWire_Init(device);
  if(ucResult == 0)
  {
    OneWire_WriteByte(device, THERM_CMD_SKIPROM);
    OneWire_WriteByte(device, THERM_CMD_WSCRATCHPAD);
    OneWire_WriteByte(device, 0);
    OneWire_WriteByte(device, 0);
    OneWire_WriteByte(device, mode);
    return 0;
  }
  else return 1;
  
}

/*****************************************************************************
*
*   Имя функции : DS18B20_SrartConvTemp
*
*   Возвращаемые значения : Нет.                          
*				    
*   Параметры : Нет.               
*
*   Задача : Функция дает команду на начало преобразования температуры для
*            единственного DS18B20 на шине.
*            
*            
*****************************************************************************/ 

void DS18B20_StartConvTemp(OW_Device * device)
{
   OneWire_SkipRom(device);                               
   OneWire_WriteByte(device, CONVERT_TEMP); 
}

/*****************************************************************************
*
*   Имя функции : DS18B20_GetTempToInt10
*
*   Возвращаемые значения : Знаковое число умноженное на 10                          
*				    
*   Параметры : Нет.               
*
*   Задача : Функция преобразует текущее содержание структуры ds18b20 символьное значение.
*                        
*            
*            
*****************************************************************************/   
void DS18B20_GetTempToInt10(OW_Device * device)
{
  
uint8_t tmpcrc;
  
int result;
unsigned char Sig;
unsigned char temperature;
unsigned int tmp;

unsigned char ram[2];

OneWire_SkipRom(device);
  
            //Глобальная переменная CRC объявлена в My_OneWire.h
  CRC = 0; 	//Перед началом очередного считывания ОЗУ датчика, ее следует обнулять.
 
  OneWire_WriteByte(device, THERM_CMD_RSCRATCHPAD); //Команда чтения ОЗУ DS18B20. 

    for(uint8_t i=0; i<9; i++)                //ОЗУ DS18B20 содержит 9 байт, поэтому у нас 9 циклов.
    {
      uint8_t t = OneWire_ReadByte(device);
       //Поочередно заполняем структуру DS18B20_ScratchPad содержанием ОЗУ датчика.
      if(i == 0) ram[0] = t; 
      if(i == 1) ram[1] = t;
      if(i == 8) tmpcrc = t;
       
      if(i < 8)  //Первые 8-байт отправляем на алгоритм подсчета контрольной суммы.              
        CRC8(t);
    }

    if(CRC == tmpcrc) //Если посчитанная МК сумма совпадает с той что была переданна 9 байтом - вернем 0.                                                                     
    { 
      device->Error = 0;
    }
    else               //Если нет, возможно при передаче произошла ошибка, следовательно данные неверны,
    {
      device->Error = 0;
    }
    
 if ((ram[1]&128) == 0)
    {
    //проверяем старший разряд 
    Sig=0;
    }
 else
    {
    Sig=1;
    // «склеиваем» нулевой и первый байты ОЗУ датчика
    tmp = ((unsigned int)ram[1]<<8)|ram[0]; 
      
    //выполняем операцию логического отрицания
    tmp = ~tmp + 1;  
 
    //помещаем результат в соответствующие переменные
    ram[0] = tmp;
    ram[1] = tmp>>8;
      
    }
//выводим значение целое знач. температуры 
temperature = (ram[0]>>4)|((ram[1]&7)<<4); 
 
result = temperature * 10;

//выводим дробную часть знач. температуры
    
temperature = (ram[0]&15);
temperature = (temperature<<1) + (temperature<<3);
temperature = (temperature>>4);
 
result = result + temperature;
 

if (Sig) result = -result;
device->Temperature10 = result;
  
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
int DS18B20_ConvTempToInt10(OW_Device * device)
{
int result;
unsigned char Sig;
unsigned char temperature;
unsigned int tmp;

unsigned char ram[2];
ram[0]=device->tempLsb;
ram[1]=device->tempMsb; 
 
 if ((ram[1]&128) == 0)
    {
    //проверяем старший разряд 
    Sig=0;
    }
 else
    {
    Sig=1;
    // «склеиваем» нулевой и первый байты ОЗУ датчика
    tmp = ((unsigned int)ram[1]<<8)|ram[0]; 
      
    //выполняем операцию логического отрицания
    tmp = ~tmp + 1;  
 
    //помещаем результат в соответствующие переменные
    ram[0] = tmp;
    ram[1] = tmp>>8;
      
    }
//выводим значение целое знач. температуры 
temperature = (ram[0]>>4)|((ram[1]&7)<<4); 
 
result = temperature * 10;

//выводим дробную часть знач. температуры
    
temperature = (ram[0]&15);
temperature = (temperature<<1) + (temperature<<3);
temperature = (temperature>>4);
 
result = result + temperature;
 

if (Sig) result = -result;
device->Temperature10 = result;
return result;
}
*/


/*

unsigned char DS18B20_ReadScratchPad(void)
{
  unsigned char i; 
  unsigned char *ptr = (unsigned char*) &ds18b20[ucActiveSensor]; //указатель для доступа к структуре ds18b20. 
  
            //Глобальная переменная CRC объявлена в My_OneWire.h
  CRC = 0; 	//Перед началом очередного считывания ОЗУ датчика, ее следует обнулять.
 
  OneWire_WriteByte(&OWCurrentDevice, THERM_CMD_RSCRATCHPAD); //Команда чтения ОЗУ DS18B20. 

    for(i=0; i<9; i++)                //ОЗУ DS18B20 содержит 9 байт, поэтому у нас 9 циклов.
    {
       *(ptr+i) = OneWire_ReadByte(&OWCurrentDevice); //Поочередно заполняем структуру DS18B20_ScratchPad содержанием ОЗУ датчика.

        if(i < 8)  	                  //Первые 8-байт отправляем на алгоритм подсчета контрольной суммы.              
           CRC8(*(ptr+i));
    }

    if(CRC == ds18b20[ucActiveSensor].crc) //Если посчитанная МК сумма совпадает с той что была переданна 9 байтом - вернем 10.                                                                     
       return 0;
        else               //Если нет, возможно при передаче произошла ошибка, следовательно данные неверны,
	      return 1;        //возвращаем 1.
}

*/


/*****************************************************************************
*
*   Имя функции : DS18B20_ReadScratchPad 
*
*   Возвращаемые значения : 1 - если при чтении содержимого ОЗУ произошли ошибки
*                           (сумма CRC принятая от датчика не совпадает с посчитанной
*                           в МК). 0 - в случае если все благополучно.                           
*				    
*   Параметры : Нет.
*
*   Задача : Чтение содержимого ОЗУ DS18B20 в массив DS18B20_ScratchPad. Все 8 
*            входящих байт проверяются на честность с CRC (9-й принятым байтом).
*            
*            
*****************************************************************************/ 
/*
unsigned char DS18B20_ReadScratchPad(OW_Device * device)
{

  uint8_t tmpcrc;
  
            //Глобальная переменная CRC объявлена в My_OneWire.h
  CRC = 0; 	//Перед началом очередного считывания ОЗУ датчика, ее следует обнулять.
 
  OneWire_WriteByte(device, THERM_CMD_RSCRATCHPAD); //Команда чтения ОЗУ DS18B20. 

    for(uint8_t i=0; i<9; i++)                //ОЗУ DS18B20 содержит 9 байт, поэтому у нас 9 циклов.
    {
      uint8_t t = OneWire_ReadByte(device);
       //Поочередно заполняем структуру DS18B20_ScratchPad содержанием ОЗУ датчика.
      if(i == 0) device->tempLsb = t; 
      if(i == 1) device->tempMsb = t;
      if(i == 8) tmpcrc = t;
       
      if(i < 8)  //Первые 8-байт отправляем на алгоритм подсчета контрольной суммы.              
        CRC8(t);
    }

    if(CRC == tmpcrc) //Если посчитанная МК сумма совпадает с той что была переданна 9 байтом - вернем 0.                                                                     
    { 
      device->Error = 0;
      return 0;
    }
    else               //Если нет, возможно при передаче произошла ошибка, следовательно данные неверны,
    {
      device->Error = 0;
      return 1;        //возвращаем 1.
    }
    }
*/
/*****************************************************************************
*
*   Имя функции : DS18B20_GetTemp 
*
*   Возвращаемые значения : При успешном окончании процесса передачи данных 
*                           из ОЗУ DS18B20 в МК, функция возвращает 0, иначе 1.
*
*   Параметры : Нет.               
*
*   Задача : После преобразования температуры (DS18B20_ConvTemp) функция читает 
*            содержимое ОЗУ DS18B20 в массив DS18B20_ScratchPad (в том числе 
*            и температуру). 
*            
*            
*****************************************************************************/ 
/*
unsigned char DS18B20_GetTemp(OW_Device * device)
{
   OneWire_SkipRom(device);     
                               
   if(DS18B20_ReadScratchPad(device) == 0)  
      return 0;
	   else
	     return 1;
}
*/


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



#endif
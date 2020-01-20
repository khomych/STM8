#include "stm8s_conf.h"
//#include "main.h"
//#include "stdio.h"
//#include "string.h"

#include "lib_ds18b20.h"
#include "lib_1wire.h"
#include "main.h"


uint8_t CRC; //Переменная для вычисления контрольной суммы.
const unsigned char CRC_Table[] =
{   0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
	157,195, 33,127,252,162, 64, 30, 95, 1,227,189, 62, 96,130,220,
	35,125,159,193, 66, 28,254,160,225,191, 93, 3,128,222, 60, 98,
	190,224, 2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
	70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89, 7,
	219,133,103, 57,186,228, 6, 88, 25, 71,165,251,120, 38,196,154,
	101, 59,217,135, 4, 90,184,230,167,249, 27, 69,198,152,122, 36,
	248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91, 5,231,185,
	140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
	17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
	175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
	50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
	202,148,118, 40,171,245, 23, 73, 8, 86,180,234,105, 55,213,139,
	87, 9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
	233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
	116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
};


/*****************************************************************************
*
*   Имя функции : DS18B20_Init
*
*   Возвращаемые значения : Нет.                          
*				    
*   Параметры : OW_Device * device - указатель на структуру               
*
*   Задача : Функция инициализирует шину 1-wire
*            
*            
*            
*****************************************************************************/
void DS18B20_Init(OW_Device * device)
{
  device->GPIOx = THERM0_PORT;
  device->GPIO_Pin = THERM0_PIN;
  
}

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
      device->Error = 1;
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

/*****************************************************************************
*
*   Имя функции : CRC8 
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Байт для проверки. 
*
*   Задача : Эта функция вычисляет контрольную сумму по стандартному алгоритму для устройств 1-Wire.
*            Переменная x принимает только один байт. Для расчета контрольной суммы 8-байт, функцию 
*            нужно вызывать для каждого из принятый байтов поочередно.     
*            
*
*****************************************************************************/ 

void CRC8(unsigned char x)   
{					  
   CRC = CRC_Table[CRC ^ x];       //Расчет заключается в простом извлечении результата из массива.
}
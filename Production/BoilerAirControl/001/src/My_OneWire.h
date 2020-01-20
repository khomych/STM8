



#ifndef MY_ONEWIRE_H
#define MY_ONEWIRE_H

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!													   !! 
//Прежде чем включать этот файл в ваш проект, не забудьте перед ним обьявить 	 !!													   !!
//адресс порта к которому подключенно ваше устройство.				             !!
//Пример для PORTD.0:				   						                     !!
//   							                                                 !!
//  #define ONE_WIRE_PIN     0     						                         !!
//  #define ONE_WIRE_PORTIN  PIND  						                         !!
//  #define ONE_WIRE_PORT    PORTD 							                     !!
//  #define ONE_WIRE_DDR     DDRD  							                     !!
//   							  						                         !!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//#define THERM_PORT GPIOD
//#define THERM_PIN  GPIO_PIN_4
 
//#define THERM_INPUT_MODE()  THERM_PORT->DDR &= ~THERM_PIN
//#define THERM_OUTPUT_MODE() THERM_PORT->DDR |=  THERM_PIN
//#define THERM_LOW()         THERM_PORT->ODR &= (u8)(~THERM_PIN)
//#define THERM_HIGH()        THERM_PORT->ODR |= (u8)THERM_PIN
//#define THERM_READ()        (THERM_PORT->IDR & (u8)THERM_PIN)

//#define THERM_PORT GPIOD
//#define THERM_PIN  GPIO_PIN_4

#define THERM1_PORT GPIOA
#define THERM1_PIN  GPIO_PIN_2
#define OW_OUTDOOR 1

#define THERM0_PORT GPIOA
#define THERM0_PIN  GPIO_PIN_1
#define OW_BASEMOUNT 0


#define ONE_WIRE_RESET          device->GPIOx->DDR |=  device->GPIO_Pin; device->GPIOx->ODR &= (~device->GPIO_Pin);  //Установка шины обмена данными в низкий уровень. 
#define ONE_WIRE_SET            device->GPIOx->DDR &= ~device->GPIO_Pin //Возврашение шины в первоначальное (высокое) состояние. 
#define ONE_WIRE_READ           device->GPIOx->IDR & (uint8_t)device->GPIO_Pin

////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void x_delay(unsigned int i) 
{ 
  while (--i) nop();
}

#define delay_1us()   nop();nop();nop();nop();nop();nop();nop();nop();
#define delay_15us()  x_delay(20)//42
#define delay_45us()  x_delay(127)//127
#define delay_60us()  x_delay(170)//170
#define delay_90us()  x_delay(255)//255
#define delay_480us() x_delay(1360)//1360


typedef struct{
  GPIO_TypeDef* GPIOx;
  GPIO_Pin_TypeDef GPIO_Pin;
  int16_t Temperature10;
  uint8_t Error;
  
}OW_Device;

//OW_Device OWCurrentDevice;

OW_Device OWBasement, OWOutdoor;


//-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;
//---------------------------								                    //;
// СТАНДАРТНЫЕ ПЗУ КОМАНДЫ :|								                    //;
//--------------------------- 								                    //;
#define READ_ROM		0x33 //Прочесть содержимое ПЗУ уст-ва.		        //;
#define MATCH_ROM		0x55 //Адрессовать на шине конкретн. уст-во.	    //;
#define SKIP_ROM		0xCC //Пропустить процедуру сравнив. сер. номера.  //;
#define SEARCH_ROM		0xF0 //Поиск всех устройств на шине.		        //;
#define CONDITIONAL_SEARCH	0xEC //						                    //;
#define READ_MEMORY		0xAA //							                //;
#define WRITE_MEMORY		0x55 //						                    //; 
//-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;-;



unsigned char ucActiveSensor = 0;

unsigned char CRC; //Переменная для вычисления контрольной суммы.

//Нижеследующий массив содержит заранее вычисленные сигнатуры, используемые для быстрого расчета CRC.
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

//----------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////ИСПОЛЬЗУЕМЫЕ ФУНКЦИИ./////////////////////////////////////////////////////////|
//----------------------------------------------------------------------------------------------------------------------




unsigned char OneWire_Init(OW_Device * device);                         
unsigned char OneWire_ReadBit(OW_Device * device);
unsigned char OneWire_ReadByte(OW_Device * device);
void OneWire_WriteBit(OW_Device * device, unsigned char data_bit);
void OneWire_WriteByte(OW_Device * device, unsigned char data);
void OneWire_ReadRom(unsigned char *id);
void OneWire_MatchRom(unsigned char *id); 
void OneWire_SkipRom(OW_Device * device);			  
void CRC8(unsigned char x);
								
//----------------------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////|
//----------------------------------------------------------------------------------------------------------------------


/*****************************************************************************
*
*   Имя функции : OneWire_Init
*
*   Возвращаемые значения : 1 - если функция инициализации прошла успешно (на шине есть устр-ва и временные
*                           рамки работы интерфейса 1-Wire соблюдены), 0 - в случае отсутствия ведомых устр-в 
*				            или проблем в работе шины.  
*
*   Параметры : Нет.
*
*   Задача : Функция инициализации обмена данными. Любой обмен информацией между ведомым и ведущим 
*            устр-вом начинается с этой функции. 
*            
*
*****************************************************************************/ 
unsigned char OneWire_Init(OW_Device * device)
{
  unsigned char i = 0xFF;
 
  ONE_WIRE_RESET;

  //delay_10us(48);
  delay_480us();
 
  ONE_WIRE_SET;
 
  //delay_10us(6);
  delay_60us();
 
  i = ONE_WIRE_READ;
 
  //delay_10us(48);
  delay_480us();
  // 0 означает правильный сброс, 1 - ошибка
  if(i == 0) return 0;
  else return 1;
}


/*****************************************************************************
*
*   Имя функции : OneWire_WriteBit
*
*   Возвращаемые значения : Нет. 
*
*   Параметры : Бит (1 или 0) подлежащий передаче.
*
*   Задача : Передача бита от ведущего устр-ва, ведомому. 
*            
*
*****************************************************************************/ 
void OneWire_WriteBit(OW_Device * device, unsigned char data_bit)
{
  disableInterrupts();
   	  
  ONE_WIRE_RESET;   
	  	
  if (data_bit)
    {
      ONE_WIRE_SET;
    }    		
  
  delay_60us();
       	
	    
  ONE_WIRE_SET;                	  					    
      
  enableInterrupts();
}


/*****************************************************************************
*
*   Имя функции : OneWire_ReadBit 
*
*   Возвращаемые значения : Принятый ведущим устр-вом бит. 
*
*   Параметры : Нет.
*
*   Задача : Прием бита от ведомого устр-ва.  
*            
*
*****************************************************************************/ 
unsigned char OneWire_ReadBit(OW_Device * device)
{
   unsigned char bBit = 0;
   
   disableInterrupts();

   ONE_WIRE_RESET; 
   delay_1us();
   ONE_WIRE_SET;   		
   delay_15us();  
   //delay_15us();

   if(ONE_WIRE_READ) bBit = 1;
                       						   						  
   delay_45us();	          
   
   enableInterrupts();

   return bBit;        
}


/*****************************************************************************
*
*   Имя функции : OneWire_WriteByte
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Байт для передачи.
*
*   Задача : Передача байта от ведущего устр-ва, ведомому. 
*            
*
*****************************************************************************/ 

void OneWire_WriteByte(OW_Device * device, unsigned char data)
{ 
 unsigned char i = 8;
 
  while(i--)
  {
    OneWire_WriteBit(device, data & 0x01);
    data >>=1;
  }
    
}
  
/*****************************************************************************
*
*   Имя функции : OneWire_ReadByte
*
*   Возвращаемые значения : Принятый ведущим устр-вом байт. 
*
*   Параметры : Нет.
*
*   Задача : Прием байта от ведомого устр-ва. 
*            
*
*****************************************************************************/ 

unsigned char OneWire_ReadByte(OW_Device * device)
{
   char i = 8;
   unsigned char Data = 0;
   
   while(i--)
   {
     Data >>=1;
     Data |= OneWire_ReadBit(device) << 7;
   }
	
   
   return Data;        //Возвращаем принятый байт.
}




/*****************************************************************************
*
*   Имя функции : OneWire_SkipRom
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет. 
*
*   Задача : Функция пропуска сравнивания серийных номеров. Команды следующие за этой функцией 
*            будут адрессоваться всем устр-вам на шине одновременно. 
*            
*
*****************************************************************************/ 

void OneWire_SkipRom(OW_Device * device)
{   
   OneWire_Init(device);
   OneWire_WriteByte(device, SKIP_ROM);  
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


//*****************************************************************************

/*
   
unsigned char OneWire_Init(void)
{
  unsigned char i = 0xFF;
 
  ONE_WIRE_RESET;

  //delay_10us(48);
  delay_480us();
 
  ONE_WIRE_SET;
 
  //delay_10us(6);
  delay_60us();
 
  i = ONE_WIRE_READ;
 
  //delay_10us(48);
  delay_480us();
  // 0 означает правильный сброс, 1 - ошибка
  if(i == 0) return 0;
  else return 1;
}




void OneWire_WriteBit(unsigned char data_bit)
{
  disableInterrupts();
   	  
  ONE_WIRE_RESET;   
	  	
  if (data_bit)
    {
      ONE_WIRE_SET;
    }    		
  
  delay_60us();
       	
	    
  ONE_WIRE_SET;                	  					    
      
  enableInterrupts();
}



unsigned char OneWire_ReadBit(void)
{
   unsigned char bBit = 0;
   
   disableInterrupts();

   ONE_WIRE_RESET; 
   delay_1us();
   ONE_WIRE_SET;   		
   delay_15us();   

   if(ONE_WIRE_READ) bBit = 1;
                       						   						  
   delay_45us();	          
   
   enableInterrupts();

   return bBit;        
}

*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*    
unsigned char OneWire_Init(void)
{
  unsigned char i = 0xFF;
 
  //ONE_WIRE_RESET;
  switch (ucActiveSensor)
  {
  case 0:
    ONE_WIRE_0_RESET;
    break;
    
  case 1:
    ONE_WIRE_1_RESET;
    break;
    
  default:
    return 1;
    break;
  }
  
  //delay_10us(48);
  delay_480us();
 
  //ONE_WIRE_SET;
    switch (ucActiveSensor)
  {
  case 0:
    ONE_WIRE_0_SET;;
    break;
    
  case 1:
    ONE_WIRE_1_SET;;
    break;
    
  default:
    return 1;
    break;
  }
 
  //delay_10us(6);
  delay_60us();
 
  //i = ONE_WIRE_READ;
      switch (ucActiveSensor)
  {
  case 0:
    i = ONE_WIRE_0_READ;
    break;
    
  case 1:
    i = ONE_WIRE_0_READ;
    break;
    
  default:
    return 1;
    break;
  }
 
  //delay_10us(48);
  delay_480us();
  // 0 означает правильный сброс, 1 - ошибка
  if(i == 0) return 0;
  else return 1;
}
*/

/*
void OneWire_WriteBit(unsigned char data_bit)
{
  disableInterrupts();
   	  
  //ONE_WIRE_RESET;   
      switch (ucActiveSensor)
  {
  case 0:
    ONE_WIRE_0_RESET;
    break;
    
  case 1:
    ONE_WIRE_1_RESET;
    break;
    
  default:
    //return 1;
    break;
  }
	  	
  if (data_bit)
    {
      //ONE_WIRE_SET;
      switch (ucActiveSensor)
        {
          case 0:
          ONE_WIRE_0_SET;
          break;
    
          case 1:
          ONE_WIRE_1_SET;
          break;
    
          default:
          //return 1;
          break;
      }
    }    		
  
  delay_60us();
       	
	    
  //ONE_WIRE_SET;                	  					    
  switch (ucActiveSensor)
    {
      case 0:
      ONE_WIRE_0_SET;
      break;
    
      case 1:
      ONE_WIRE_1_SET;
      break;
    
      default:
      //return 1;
      break;
    }
      
  enableInterrupts();
}
*/

/*
unsigned char OneWire_ReadBit(void)
{
   unsigned char bBit = 0;
   
   disableInterrupts();

   //ONE_WIRE_RESET; 
         switch (ucActiveSensor)
        {
          case 0:
          ONE_WIRE_0_RESET;
          break;
    
          case 1:
          ONE_WIRE_1_RESET;
          break;
    
          default:
          //return 1;
          break;
      }
   delay_1us();
   //ONE_WIRE_SET;
         switch (ucActiveSensor)
        {
          case 0:
          ONE_WIRE_0_SET;
          break;
    
          case 1:
          ONE_WIRE_1_SET;
          break;
    
          default:
          //return 1;
          break;
      }
   delay_15us();   

   //if(ONE_WIRE_READ) bBit = 1;
         switch (ucActiveSensor)
        {
          case 0:
          if(ONE_WIRE_0_READ) bBit = 1;
          break;
    
          case 1:
          if(ONE_WIRE_1_READ) bBit = 1;
          break;
    
          default:
          //return 1;
          break;
      }                    						   						  
   delay_45us();	          
   
   enableInterrupts();

   return bBit;        
}
*/

#endif
/**

  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_1WIRE_H
#define __LIB_1WIRE_H

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

 

 



//Датчик обратки    
#define THERM0_PORT GPIOC
#define THERM0_PIN  GPIO_PIN_4



#define ONE_WIRE_RESET          device->GPIOx->DDR |=  device->GPIO_Pin; device->GPIOx->ODR &= (~device->GPIO_Pin);  //Установка шины обмена данными в низкий уровень. 
#define ONE_WIRE_SET            device->GPIOx->DDR &= ~device->GPIO_Pin //Возврашение шины в первоначальное (высокое) состояние. 
#define ONE_WIRE_READ           device->GPIOx->IDR & (uint8_t)device->GPIO_Pin

////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
   
    

//---------------------------
// СТАНДАРТНЫЕ ПЗУ КОМАНДЫ :|								       
//--------------------------- 								           
#define READ_ROM		0x33 //Прочесть содержимое ПЗУ уст-ва.		     
#define MATCH_ROM		0x55 //Адрессовать на шине конкретн. уст-во.	   
#define SKIP_ROM		0xCC //Пропустить процедуру сравнив. сер. номера.  
#define SEARCH_ROM		0xF0 //Поиск всех устройств на шине.		        
#define CONDITIONAL_SEARCH	0xEC //						                 
#define READ_MEMORY		0xAA //							             
#define WRITE_MEMORY		0x55 //						               
    
    
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

unsigned char OneWire_Init(OW_Device * device);                         
unsigned char OneWire_ReadBit(OW_Device * device);
unsigned char OneWire_ReadByte(OW_Device * device);
void OneWire_WriteBit(OW_Device * device, unsigned char data_bit);
void OneWire_WriteByte(OW_Device * device, unsigned char data);
void OneWire_ReadRom(unsigned char *id);
void OneWire_MatchRom(unsigned char *id); 
void OneWire_SkipRom(OW_Device * device);			  



#endif /*__LIB_1WIRE_H*/

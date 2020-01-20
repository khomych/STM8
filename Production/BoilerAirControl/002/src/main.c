#include "stm8s_conf.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

#include "lib_pwm.h"
#include "lib_1wire.h"
#include "lib_ds18b20.h"
#include "lib_max7219.h"
#include "function.h"


/**
  *inc->stm8s_conf.h - раскоментировать строчки с нужными хедерами 
  *периферии и необходимыми обработчиками прерываний. Все закоментированные
  *обработчики ведут на бесконечные циклы.
  * 
  *Project->Options->General Options - выбрать мк
  *
  *Project->Options->Debugger - выбрать отладчик
  *
  *Project->Options->C/C++ Compiler->Preprocessor->Defined symbols  - задать
  *семейство процессора(перечислены в lib->SPL->inc->stm8s.h), а также задать
  *частоты внутренних и внешних генераторов(если не задать, то будут ипользованы
  *значения по умолчанию из stm8s.h).
  */



/*
Подключение периферии:
VCC - Красный
GND - Синий
PD6 - Power Control ##GPIO IN## Розовый
PD5 - Pomp ##GPIO OUT## Зеленый
PD4 - BEEPER 
PD3 - SERVO Оранжевый + Коричневый
PD2 - Button Mode (Manual - 0, Auto - 1) ##GPIO IN##
PD1 - Button Plus ##GPIO IN##
PC7 - Button Minus ##GPIO IN##
PC6 - MOSI Display
PC5 - SCK Display
PC4 - CS Display ##GPIO OUT##
PA1 - Temp Sensor 1 Желтый
PA2 - Temp Sensor 2 Белый

*/

//*****************************************************************************
//              ENGINE SECTION
//*****************************************************************************
#define SERVO_CHANNEL PWM_C2
#define SERVO 0
#define POMP 1

bool bHisteresis = TRUE;//false - идет снижение температуры; true - рост
uint8_t u8Mode;// 0 - manual; 1 - auto1; 2 - auto2(not use in current version)
//uint8_t u8ManualParam[2]; // хранение параметров для ручного режима. они не сохраняются в eeprom

uint8_t u8AutoParam[2];// хранение параметров для автоматического режима. они не сохраняются в eeprom


bool bPowerMode = TRUE; //true - work from voltage 220, false - work from battery


//*****************************************************************************
//              MENU SECTION
//*****************************************************************************

uint8_t u8MenuMode = 0; //0 отображение температуры , 1+ - menu ithem
bool bMenuSelect = FALSE; // если true то выбираем пункты меню, если ложь то редактируем значение выбранного пункта меню

uint8_t u8MenuTimer = 0;  
#define MENU_TIME 5
#define SET_MENU_TIMER()  u8MenuTimer = MENU_TIME    

#define MENU_COUNT 6 
#define MIN_PARAM 5
#define MAX_PARAM 6
#define DEFAULT_PARAM 4

#define TEMP_MIN 0
#define TEMP_MAX 1
#define TEMP_ALARM 2
#define DROS_MIN 3 
#define DROS_MAX 4

const uint8_t u8Menu[MENU_COUNT+ 1][7] = {
0x12,0x01,0x11,0x11,30,15,99,   //t1      default 30, min 15, max 99
0x12,0x02,0x11,0x11,60,15,99,   //t2      default 60, min 15, max 99
0x12,0x13,0x14,0x15,75,15,99,   //tAlr    default 75, min 15, max 99
0x16,0x15,0x00,0x11,0,0,18,     //d0      default 0,  min 0,  max 18
0x16,0x15,0x01,0x11,18,0,18,    //d1      default 18,  min 0,  max 18
0x17,0x18,0x18,0x19,1,0,1,       //beeP    default 0,  min 0,  max 1
0x16,0x15,0x11,0x11,0,0,18       //beeP    default 0,  min 0,  max 1
};
uint8_t u8MenuParam[MENU_COUNT + 1];


//*****************************************************************************
//              DISPLAY SECTION
//*****************************************************************************
#define INFO_TIME 10          //время отображения инфы // случайно поменялись местами!!!! не исправлял
#define TEMPERATURE_TIME 2   //время отбражения температуры

bool bManualMenuVisible = FALSE;
uint8_t u8ManualMenuTimer = 0;  
#define MANUAL_MENU_TIME 5
#define SET_MANUAL_MENU_TIMER()  u8ManualMenuTimer = MANUAL_MENU_TIME 

uint8_t u8DispTimer;            //счетчик для вычисления отображения инфы/температуры

uint8_t u8DisplayBuff[8];
uint8_t u8DisplayFlash;
bool bFlash; // используется для мигания разрядами

//*****************************************************************************
//              TIMER SECTION
//*****************************************************************************
bool b_1S_Flag = FALSE; //Флаг события 1 секунда
bool b_100mS_Flag = FALSE;// флаг срабатывания таймера на 100 мс

uint8_t u8LedBuffer[8];  //буфер дисплея

//*****************************************************************************
//              KEYS SECTION
//*****************************************************************************
#define TIMER_LONG_PRESS 1000
#define KEY_NOIZE_TIME 255;

uint16_t u16Timer_P, u16Timer_M;

uint8_t u8KeyPNoizeTim, u8KeyMNoizeTim;

bool bKey_M_Old = TRUE;
bool bKey_P_Old = TRUE;

bool b_Key_M_Press_Flag = FALSE;
bool b_Key_P_Press_Flag = FALSE;
    
bool b_Key_M_Up_Flag = FALSE;
bool b_Key_P_Up_Flag = FALSE;

bool b_Key_M_Long_Press_Flag = FALSE;
bool b_Key_P_Long_Press_Flag = FALSE;
    
bool b_Key_Scan_Flag = FALSE;  


//*****************************************************************************
//              EEPROM SECTION
//*****************************************************************************
//Данные в EEPROM
#pragma location=0x4000
__no_init uint8_t u8EEMenuParam[MENU_COUNT + 1];


uint8_t u8Pwm = 0;

//Структуры с данными термодатчиков
OW_Device owdThermBoiler, owdThermReturn;
extern uint8_t CharSet[];

typedef enum{
EEPROM_READ = 0x00,
EEPROM_SAVE = 0x01
}EEPROMCmd;

//*****************************************************************************
//               Declaration Function 
//*****************************************************************************
void actionKeyScan(void);
void ShowTemp(void);
void ShowMenu(void);
void ShowInfo(uint8_t u8Param);

void actionTimer_100mS(void);
void Display(uint8_t * u8MTetrad, uint8_t * u8LTetrad, uint8_t u8Dots, uint8_t u8Flash);
void DisplayFlash(void);

void EEPROMParam(EEPROMCmd EEcmd);

void SetServo(uint8_t u8Angle);

void Engine(void);






//*****************************************************************************
//               Definition Function 
//*****************************************************************************


/*****************************************************************************
*
*   Имя функции : SetServo
*
*   Возвращаемые значения :  Нет
*                           
*
*   Параметры : 
*
*   Задача :
*            
*            
*****************************************************************************/
void SetServo(uint8_t u8Angle)
{
 
  uint8_t u8Resolution;
  
  uint16_t u16Result;
  if(u8Angle > 90)u8Angle = 90;
  
  u8Resolution = SERVO_MAX_POS - SERVO_MIN_POS;
  u16Result = u8Angle * u8Resolution / 90 + SERVO_MIN_POS;
    
  pwm_servo(PWM_SERVO, (uint8_t)u16Result);

  
  
  
}




/*****************************************************************************
*
*   Имя функции : Engine
*
*   Возвращаемые значения :  Нет
*                           
*
*   Параметры : 
*
*   Задача :
*            
*            
*****************************************************************************/
void Engine(void)
{
  uint8_t u8MinTmp, u8MaxTmp, u8AlrmTmp;
  u8MinTmp = u8MenuParam[TEMP_MIN];
  u8MaxTmp = u8MenuParam[TEMP_MAX];
  u8AlrmTmp = u8MenuParam[TEMP_ALARM];
  
  uint8_t u8CurrentTmp = owdThermBoiler.Temperature10 / 10;
  
  //read control power pin
  if(GPIO_ReadInputPin(POW_CTRL_PORT, POW_CTRL_PIN)== RESET)bPowerMode = FALSE;
  else bPowerMode = TRUE;
  
  //read key mode pin
  if(GPIO_ReadInputPin(KEY_MODE_PORT, KEY_MODE_PIN)== RESET)u8Mode = 0;
  else u8Mode = 1;
  
  
  
  if(u8CurrentTmp >= u8AlrmTmp)//ПЕРЕГРЕВ! АВАРИЙНАЯ ОСТАНОВКА!
  {
    u8AutoParam[SERVO] = 0;
    SetServo(u8AutoParam[SERVO]);
    Beep(1);
  }
  else // Штатная работа. Дальнейшие проверки
  {
    if(bPowerMode == FALSE) // work from BATT
    {
      SetServo(0);
    }
    else // work from 220
    {
      
      if(u8Mode == 0)//manual mode
      {
        SetServo(u8MenuParam[MENU_COUNT] * 5);
      }
      else // auto mode
      {
        if(u8CurrentTmp < u8MinTmp) // котел остыл. открыть дроссель, гистерезис на рост
        {
          bHisteresis = TRUE;
          u8AutoParam[SERVO] = u8MenuParam[DROS_MAX];
        }
    
        if(u8CurrentTmp >= u8MaxTmp) // котел нагрелся. дроссель закрыть, гистерезис на спад
        {
          bHisteresis = FALSE;
          u8AutoParam[SERVO] = u8MenuParam[DROS_MIN];
        }
    
        if((u8CurrentTmp >= u8MinTmp) && (u8CurrentTmp < u8MaxTmp)) //промежуточная проверка
        {
          if(bHisteresis == TRUE)u8AutoParam[SERVO] = u8MenuParam[DROS_MAX];
          else u8AutoParam[SERVO] = u8MenuParam[DROS_MIN];
        }
        
        SetServo(u8AutoParam[SERVO] * 5);
      }
    }
  }
  
}

/*****************************************************************************
*
*   Имя функции : EEPROMParam
*
*   Возвращаемые значения :  Нет
*                           
*
*   Параметры : EEPROMCmd EEcmd. EEPROM_READ, EEPROM_SAVE
*
*   Задача :
*            
*            
*****************************************************************************/
void EEPROMParam(EEPROMCmd EEcmd)
{
  bool bEESave = FALSE;
  
  switch (EEcmd)
  {
  case EEPROM_READ:
  
    //загрузка данных с EEPROM
    for(uint8_t i = 0; i < MENU_COUNT + 1; i++)
    {
      u8MenuParam[i] = u8EEMenuParam[i]; 
    }
    
    //проверка данных на валидность
    for(uint8_t i = 0; i < MENU_COUNT + 1; i++)
    {
      if((u8MenuParam[i] < u8Menu[i][MIN_PARAM]) | (u8MenuParam[i] > u8Menu[i][MAX_PARAM]))
      {
        u8MenuParam[i] = u8Menu[i][DEFAULT_PARAM];
      } 
    }
        
    break;
    
  case EEPROM_SAVE:
    
      for(uint8_t i = 0; i < MENU_COUNT + 1; i++)//проверить, стоит ли зависывать в EEPROM
      {
        if(u8MenuParam[i] != u8EEMenuParam[i])bEESave = TRUE; 
      }
    if(bEESave == TRUE)
    {
      disableInterrupts();
      FLASH_Unlock(FLASH_MEMTYPE_DATA);
      
      for(uint8_t i = 0; i < MENU_COUNT + 1; i++)
        {
          if(u8MenuParam[i] != u8EEMenuParam[i])u8EEMenuParam[i] = u8MenuParam[i]; 
        }
      
      FLASH_Lock(FLASH_MEMTYPE_DATA);
      enableInterrupts();
      
    }

    break;
    
  default:
    
    break;
  }
}




/*****************************************************************************
*
*   Имя функции : Display
*
*   Возвращаемые значения :  Нет
*                           
*
*   Параметры : uint8_t * u8MTetrad - старшие (левые) четыре значения дисплея
*               uint8_t * u8LTetrad - младшие (правые) четыре значения дисплея
*               uint8_t u8Dots - каждый бит включает соответствующую точку дисплея
*               uint8_t u8Flash - каждый бит включает мигание соответствующим разрядом дисплея
*
*   Задача :
*            
*            
*****************************************************************************/
void Display(uint8_t * u8MTetrad, uint8_t * u8LTetrad, uint8_t u8Dots, uint8_t u8Flash)
{
    
  u8DisplayFlash = u8Flash;
  for(uint8_t i = 0; i < 8; i++)
  {
    if(i <= 3)u8DisplayBuff[i] = u8LTetrad[i];
    else u8DisplayBuff[i] = u8MTetrad[i - 4];
    
    u8DisplayBuff[i] = CharSet[u8DisplayBuff[i]];
    if(u8Dots & (1 << i))u8DisplayBuff[i] = u8DisplayBuff[i] | 0x80;//set dot
    

    
    MAX7219_Shift(i + 1, u8DisplayBuff[i]);
    
  }
}

/*****************************************************************************
*
*   Имя функции : DisplayFlash
*
*   Возвращаемые значения :  Нет
*                           
*
*   Параметры : 
*
*   Задача : при необходимости мигать разрядами дисплея.
*            u8DisplayFlash хранит инвормацию о нужных разрядах
*            в фикле 100 мс делать проверку if(u8DisplayFlash)DisplayFlash();
*            
*            
*****************************************************************************/
void DisplayFlash(void)
{
  for(uint8_t i = 0; i < 8; i++)
  {
    
    if(u8DisplayFlash & (1 << i))
    {
      if(bFlash == TRUE)
      {
        MAX7219_Shift(i + 1, 0);
      }
      else
      {
        MAX7219_Shift(i + 1, u8DisplayBuff[i]);
      }
  }
  
}
}

//*****************************************************************************
//              actionKeyScan function
//*****************************************************************************
void actionKeyScan(void)
{
//////////////////////////////////////////////////////////////////////////////
  //Сканирование кнопок
  //////////////////////////////////////////////////////////////////////////////
  
  if((KEY_P_PORT->IDR & KEY_P_PIN) == 0) // Кнопка +
  {
    b_Key_P_Up_Flag = FALSE;
    if((u8KeyPNoizeTim == 0) && (bKey_P_Old == TRUE))
    {
      //Произошло нажатие кнопки
      if(u8MenuParam[5] == 1) Beep(1);//************************BEEP*********************************
      
      u8KeyPNoizeTim = KEY_NOIZE_TIME;//атидребезг
      
      bKey_P_Old = FALSE;
      //Запуск обработчика
      u16Timer_P = 1;
      b_Key_P_Press_Flag = TRUE;
    }
    else
    {
      //Кнопка зажатаб можно считать время зажатия
      if(u16Timer_P > 0)
      {
        if(u16Timer_P > TIMER_LONG_PRESS)
        {
          u16Timer_P = 0;
          b_Key_P_Long_Press_Flag = TRUE;
        }
        else u16Timer_P++;
      }
      
    }
  }
  else
  {
    if(bKey_P_Old == FALSE)
    {
      //Произошло отпускание кнопки
      b_Key_P_Up_Flag = TRUE;
      bKey_P_Old = TRUE;
      u16Timer_P = 0;
    }
  }
  
  //////////////////////////////////////////////////////////////////////////////
  
  if((KEY_M_PORT->IDR & KEY_M_PIN) == 0) // Кнопка -
  {
    b_Key_M_Up_Flag = FALSE;
    if((u8KeyMNoizeTim == 0) && (bKey_M_Old == TRUE))
    {
      //Произошло нажатие кнопки
      if(u8MenuParam[5] == 1) Beep(1);//************************BEEP*********************************
      
      u8KeyMNoizeTim = KEY_NOIZE_TIME;//атидребезг
      
      bKey_M_Old = FALSE;
      //Запуск обработчика
      u16Timer_M = 1;
      b_Key_M_Press_Flag = TRUE;
    }
    else
    {

      //Кнопка зажатаб можно считать время зажатия
      if(u16Timer_M > 0)
      {
        if(u16Timer_M > TIMER_LONG_PRESS)
        {
          u16Timer_M = 0;
          b_Key_M_Long_Press_Flag = TRUE;
        }
        else u16Timer_M++;
      }
      
    }
  }
  else
  {
    if(bKey_M_Old == FALSE)
    {
      //Произошло отпускание кнопки
     
      
      b_Key_M_Up_Flag = TRUE;
      bKey_M_Old = TRUE;
      u16Timer_M = 0;
    }
  }
  //////////////////////////////////////////////////////////////////////////////

  
  if(u8KeyMNoizeTim > 0)u8KeyMNoizeTim--;
  if(u8KeyPNoizeTim > 0)u8KeyPNoizeTim--;
  b_Key_Scan_Flag = FALSE;
}    
 
/*****************************************************************************
*
*   Имя функции : actionKey_P_Press
*
*   Возвращаемые значения :  Нет
*                           
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionKey_P_Press(void)
{
   
  if(u8Mode == 0)//Manual Mode
  {
    
    if(u8ManualMenuTimer == 0)SET_MANUAL_MENU_TIMER();
    else
    {
      SET_MANUAL_MENU_TIMER();
      u8MenuParam[MENU_COUNT]++;
      if(u8MenuParam[MENU_COUNT] > u8Menu[MENU_COUNT][6])
      {
        u8MenuParam[MENU_COUNT] = u8Menu[MENU_COUNT][5];
      }
    }
  }
  else //Auto Mode #1
  {
    if(u8MenuMode > 0)SET_MENU_TIMER();
   
    if(bMenuSelect == TRUE)// перелистываем меню
    {
      u8MenuMode++;
      if(u8MenuMode > MENU_COUNT)u8MenuMode = 1;
      ShowMenu();
    }
    else //редактируем выбранный параметр
    {
      u8MenuParam[u8MenuMode - 1] = u8MenuParam[u8MenuMode - 1] + 1;
      if(u8MenuParam[u8MenuMode - 1] > u8Menu[u8MenuMode - 1][6])
      {
        u8MenuParam[u8MenuMode - 1] = u8Menu[u8MenuMode - 1][5];
      }
    }
  }
  


b_Key_P_Press_Flag = FALSE;  
}

/*****************************************************************************
*
*   Имя функции : actionKey_M_Press
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionKey_M_Press(void)
{
  
  if(u8Mode == 0)//Manual Mode
  {
    
    if(u8ManualMenuTimer == 0)SET_MANUAL_MENU_TIMER();
    else
    {
      SET_MANUAL_MENU_TIMER();
      u8MenuParam[MENU_COUNT]--;
      if(u8MenuParam[MENU_COUNT] > u8Menu[MENU_COUNT][MAX_PARAM])
      {
        u8MenuParam[MENU_COUNT] = u8Menu[MENU_COUNT][MAX_PARAM];
      }
    }
  }
  else //Auto Mode #1
  {
    if(u8MenuMode > 0)SET_MENU_TIMER();
   
    if(bMenuSelect == TRUE)// перелистываем меню
    {
     
    }
   
    else //редактируем выбранный параметр
    {
      u8MenuParam[u8MenuMode - 1] = u8MenuParam[u8MenuMode - 1] - 1;
      if((u8MenuParam[u8MenuMode - 1] < u8Menu[u8MenuMode - 1][MIN_PARAM]) || 
      (u8MenuParam[u8MenuMode - 1] > u8Menu[u8MenuMode - 1][MAX_PARAM]))
      {
        u8MenuParam[u8MenuMode - 1] = u8Menu[u8MenuMode - 1][MAX_PARAM];
      }
    }
  }
   
  
   
   
b_Key_M_Press_Flag = FALSE;
}

/*****************************************************************************
*
*   Имя функции : actionKey_P_Long_Press
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionKey_P_Long_Press(void)
{
  
  if(u8Mode == 0)//Manual Mode
  {
    
  }
  else //Auto Mode #1
  {
    
  }

  b_Key_P_Long_Press_Flag = FALSE;
}

/*****************************************************************************
*
*   Имя функции : actionKey_M_Long_Press
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionKey_M_Long_Press(void)
{
  
  if(u8Mode == 0)//Manual Mode
  {
    
  }
  else //Auto Mode #1
  {
    SET_MENU_TIMER();
    u8MenuMode = 1;
    bMenuSelect = TRUE;
  }

b_Key_M_Long_Press_Flag = FALSE;
}

/*****************************************************************************
*
*   Имя функции : actionKey_M_Long_Press
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionKey_M_Up(void)
{
   
   bMenuSelect = FALSE;
   b_Key_M_Up_Flag = FALSE;
}

/*****************************************************************************
*
*   Имя функции : ShowTemp
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void ShowTemp(void)
{
  //int temp[2];owdThermBoiler owdThermReturn
  int16_t tempBoiler, tempReturn;
  uint8_t d[3];
  
  uint8_t Mbuff[4];
  uint8_t Lbuff[4];
  uint8_t u8Dots = 0;
       

  DS18B20_GetTempToInt10(&owdThermBoiler);
  tempBoiler = owdThermBoiler.Temperature10;
  DS18B20_StartConvTemp(&owdThermBoiler);
              

  DS18B20_GetTempToInt10(&owdThermReturn);
  tempReturn = owdThermReturn.Temperature10;
  DS18B20_StartConvTemp(&owdThermReturn);
        
        
        
  if(tempBoiler < 0)
  {
    Mbuff[3] = 0x10; // знак минус
    tempBoiler = -tempBoiler;
  }
  else Mbuff[3] = 0x11; // пусто
  d[2] = tempBoiler / 100;
  d[1] = (tempBoiler - d[2] * 100) / 10;
  d[0] = tempBoiler - d[2] * 100 - d[1] * 10;
        
  Mbuff[2] = d[2]; 
  Mbuff[1] = d[1]; 
  Mbuff[0] = d[0]; 
  u8Dots = u8Dots | 0x20;

        
  if(tempReturn < 0)
  {
    Lbuff[3] = 0x10; // знак минус
    tempReturn = -tempReturn;
  }
  else Lbuff[3] = 0x11;// пусто
  d[2] = tempReturn / 100;
  d[1] = (tempReturn - d[2] * 100) / 10;
  d[0] = tempReturn - d[2] * 100 - d[1] * 10;
        
  Lbuff[2] = d[2]; 
  Lbuff[1] = d[1]; 
  Lbuff[0] = d[0]; 
  u8Dots = u8Dots | 0x02; 
 
    Display(Mbuff, Lbuff, u8Dots, 0);
}


/*****************************************************************************
*
*   Имя функции : ShowMenu
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void ShowMenu(void)
{
  
  //u8MenuMode //0 отображение температуры , 1+ - menu ithem
  // bMenuSelect // если true то выбираем пункты меню, если ложь то редактируем значение выбранного пункта меню
  
  uint8_t Mbuff[4] = {0x11,0x11,0x11,0x11};
  uint8_t Lbuff[4] = {0x11,0x11,0x11,0x11};
  uint8_t u8Flash = 0;
  uint8_t d[2]; // для рассчетов
  uint8_t u8Param;
      
   if(bMenuSelect == TRUE) //выбор меню - мигаем пунктом меню
   {
      u8Flash = 0xF0;
   }
   else
   {
     u8Flash = 0x0F;
   }
     
   Mbuff[3] = u8Menu[u8MenuMode - 1][0];
   Mbuff[2] = u8Menu[u8MenuMode - 1][1];
   Mbuff[1] = u8Menu[u8MenuMode - 1][2];
   Mbuff[0] = u8Menu[u8MenuMode - 1][3];
   
   
   //u8MenuParam
   
   u8Param = u8MenuParam[u8MenuMode - 1];

   d[1] = u8MenuParam[u8MenuMode - 1] / 10;
   d[0] = u8MenuParam[u8MenuMode - 1] -  d[1] * 10;
   
   if(d[1] == 0)d[1] = 0x11;
   
   //Lbuff[3] = 0x11;
   Lbuff[2] = d[1];
   Lbuff[1] = d[0];
   Lbuff[0] = 0x11;
   
   
   switch (u8MenuMode-1)
   {
   case 1:
     
     break;
     
   case 2:
     
     break;
     
   case 3:
     
     u8Param = u8Param * 5;
     d[1] = u8Param / 10;
     d[0] = u8Param -  d[1] * 10;
   
     if(d[1] == 0)d[1] = 0x11;
     Lbuff[3] = 0x11;
     Lbuff[2] = d[1];
     Lbuff[1] = d[0];
     Lbuff[0] = 0x11;
   
     SetServo(u8Param);
     break;
     
   case 4:
     u8Param = u8Param * 5;
     
     d[1] = u8Param / 10;
     d[0] = u8Param -  d[1] * 10;
   
     if(d[1] == 0)d[1] = 0x11;
     Lbuff[3] = 0x11;
     Lbuff[2] = d[1];
     Lbuff[1] = d[0];
     Lbuff[0] = 0x11;
   
     SetServo(u8Param);
     break;
     
   case 5:
     if(u8MenuParam[u8MenuMode - 1] == 1)
     {
       Lbuff[2] = 0;           //print On
       Lbuff[1] = 0x1A;
     }
     else
     {
       Lbuff[2] = 0;           //print OFF
       Lbuff[1] = 0x1B;
       Lbuff[0] = 0x1B;
     }
     break;
   }


   Display(Mbuff, Lbuff, 0, u8Flash);

}

/*****************************************************************************
*
*   Имя функции : ShowInfo
*
*   Возвращаемые значения : Нет.
*
*   Параметры : uint8_t u8Param - номер нараметра
*
*   Задача :
*            
*            
*****************************************************************************/
void ShowInfo(uint8_t u8Param)
{
  uint8_t Mbuff[4] = {0x11,0x11,0x11,0x11};
  uint8_t Lbuff[4] = {0x11,0x11,0x11,0x11};
  uint8_t d[2]; // для рассчетов
  uint8_t tmp;
  
  switch (u8Param)
  {
  case 0:
    //u8ManualParam
    //
    if(u8Mode == 0)tmp = u8MenuParam[MENU_COUNT] * 5;
    else tmp = u8AutoParam[0] * 5;
    
    d[1] = tmp / 10;
    d[0] = tmp -  d[1] * 10;
   
    if(d[1] == 0)d[1] = 0x11;
    
    //Lbuff[3] = 0x11;
    Lbuff[2] = d[1];
    Lbuff[1] = d[0];
    //Lbuff[0] = 0x11;
    
    Mbuff[3] = 0x16;
    Mbuff[2] = 0x15;
    //Mbuff[1] = 0x11;
    //Mbuff[0] = 0x11;
    
    break;
    
  default:
    
    break;
  }
  
  //if(u8ManualMenuTimer > 0)tmp = 0x0F;
  Display(Mbuff, Lbuff, 0, 0);
}

//*****************************************************************************
//              main function
//***************************************************************************** 
void main(void)
{
  
  SysClock_Cong();
  
  GPIO_Conf();
  TIM4_Conf();
  SPI_Conf();
  
  pwm_init(PWM_SERVO, PWM_C2 );
  SetServo(0);
  
  //Настройка термодатчиков
  //owdThermBoiler, owdThermReturn;
  owdThermBoiler.GPIOx = THERM0_PORT;
  owdThermBoiler.GPIO_Pin = THERM0_PIN;
    
  owdThermReturn.GPIOx = THERM1_PORT;
  owdThermReturn.GPIO_Pin = THERM1_PIN;
      
  Ds18B20_Mode(&owdThermBoiler, THERM_MODE_12BIT);
  DS18B20_StartConvTemp(&owdThermBoiler);
             
  Ds18B20_Mode(&owdThermReturn, THERM_MODE_12BIT);
  DS18B20_StartConvTemp(&owdThermReturn);
  
  
  //загрузка параметров 
  EEPROMParam(EEPROM_READ);

  
  MAX7219_Init(0x0F);

  BEEP_Init(BEEP_FREQUENCY);
  
  enableInterrupts();
  IWDG_Config();

  while (1)
  {
    
          Beep(0);
          if(b_Key_Scan_Flag == TRUE)actionKeyScan();
          
          if(b_100mS_Flag == TRUE)actionTimer_100mS();
          if(b_1S_Flag == TRUE)actionTimer_1S();
          
          if(b_Key_M_Press_Flag == TRUE)actionKey_M_Press();
          if(b_Key_P_Press_Flag == TRUE)actionKey_P_Press();
          if(b_Key_M_Long_Press_Flag == TRUE)actionKey_M_Long_Press();
          if(b_Key_P_Long_Press_Flag == TRUE)actionKey_P_Long_Press();
          if(b_Key_M_Up_Flag == TRUE)actionKey_M_Up();

  };
}


/*****************************************************************************
*
*   Имя функции : actionTimer_100mS
*
*   Возвращаемые значения :
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionTimer_100mS(void)
{

  IWDG_ReloadCounter();
  Engine();

  if(u8MenuMode > 0)ShowMenu();
  if(u8ManualMenuTimer > 0)ShowInfo(0);
  
  
  if(bFlash == TRUE)bFlash = FALSE; // Флаг используется для генерации мигания дисплеем
  else bFlash = TRUE;
  if(u8DisplayFlash)DisplayFlash(); // мигать разрядами дисплея
   
  b_100mS_Flag = FALSE;
   
}

/*****************************************************************************
*
*   Имя функции : actionTimer_1S
*
*   Возвращаемые значения :
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void actionTimer_1S(void)
{

  
if(u8ManualMenuTimer == 1)EEPROMParam(EEPROM_SAVE);
if(u8ManualMenuTimer)u8ManualMenuTimer--;
  
if(bMenuSelect == TRUE) SET_MENU_TIMER();//пока зажата кнопка М не запускать автовыключение меню

if(u8MenuTimer == 1)EEPROMParam(EEPROM_SAVE);
if(u8MenuTimer)u8MenuTimer--;
if(u8MenuTimer == 0)u8MenuMode = 0;

if((u8ManualMenuTimer == 0) & (u8MenuMode == 0))
{
  if(u8DispTimer >= TEMPERATURE_TIME) ShowTemp();
  else ShowInfo(0);
  u8DispTimer++;
  if(u8DispTimer > (INFO_TIME + TEMPERATURE_TIME))u8DispTimer = 0;
}



  


b_1S_Flag = FALSE;
}




/*****************************************************************************
*
*   Имя функции : SysClock_Cong
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void SysClock_Cong(void)
{
       // ---------- CLK CONFIG -----------------------
    CLK_DeInit();

    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1); // set 16 MHz for CPU
    
    

    // uncomment if use HSE on Quartz
    //status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

}

/*****************************************************************************
*
*   Имя функции : SysClock_Cong
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
static void IWDG_Config(void)
{
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
  
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_128);
  
  /* Set counter reload value to obtain 250ms IWDG Timeout.
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LSI/128)
                         = 0.25s / (LsiFreq/128)
                         = LsiFreq/(128 * 4)
                         = LsiFreq/512

    from defines in project settings: LsiFreq=((uint32_t)128000)
   */
  IWDG_SetReload((uint8_t)(LsiFreq/512));
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}

/*****************************************************************************
*
*   Имя функции : TIM4_Conf
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void TIM4_Conf()
{
   
   
   TIM4_Cmd(DISABLE);       // stop
   TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);// 124(125-1): 16000000 / 128 / 125 = 1000 ==  1мс
   TIM4_ClearFlag(TIM4_FLAG_UPDATE);
   TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
   TIM4_Cmd(ENABLE);       // let's go
}
/*****************************************************************************
*
*   Имя функции : GPIO_Conf
*
*   Возвращаемые значения : Нет
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void GPIO_Conf()
{
   GPIO_DeInit(GPIOB);
   GPIO_DeInit(GPIOC);
   GPIO_DeInit(GPIOD);

   GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);       // LED PIN INIT
   
   GPIO_Init(MAX7219_CS_PORT, MAX7219_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //MAX7219_CS PIN INIT
   
   GPIO_Init(POMP_PORT, POMP_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
   
   GPIO_Init(KEY_P_PORT, KEY_P_PIN,GPIO_MODE_IN_PU_NO_IT);
   GPIO_Init(KEY_M_PORT, KEY_M_PIN,GPIO_MODE_IN_PU_NO_IT);
   
   GPIO_Init(KEY_MODE_PORT, KEY_MODE_PIN,GPIO_MODE_IN_FL_NO_IT);
   
   GPIO_Init(POW_CTRL_PORT, POW_CTRL_PIN,GPIO_MODE_IN_PU_NO_IT);
    
  
  
}

/*****************************************************************************
*
*   Имя функции : SPI_Conf
*
*   Возвращаемые значения : Нет.
*
*   Параметры : Нет.
*
*   Задача :
*            
*            
*****************************************************************************/
void SPI_Conf()
{
    /* Enable SPI clock */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

  /* Set the MOSI and SCK at high level */
  //GPIO_ExternalPullUpConfig(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_6 |GPIO_PIN_5), ENABLE);

  /* SD_SPI Configuration */
  SPI_Init( SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_1LINE_TX,
           SPI_NSS_SOFT, 0x07);


  /* SD_SPI enable */
  SPI_Cmd( ENABLE);


}






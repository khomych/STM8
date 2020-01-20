
#include "stm8s_conf.h"
#include "main.h"

#include "function.h"







//*****************************************************************************
//              BEEPER VARIABLES
//****************************************************************************
uint32_t u32BeepCounter = 0;
bool bBeepStatus = FALSE;

uint32_t u32Millis; //Счетчик миллисекунд



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
*   Параметры : (uint8_t)status. 0 - передавать в бесконечном цикле для обработки времени работы
*                                1 - одинарный Beep
*                                2 - двойной Beep
*
*   Задача : Beep
*            
*            
*****************************************************************************/
void Beep(uint8_t status)
{
  switch (status)
  {
  case 0:
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
    
  case 1:
    if(bBeepStatus == FALSE)
    {
      BEEP_Cmd(ENABLE);
      bBeepStatus = TRUE;
    }
    u32BeepCounter = Millis() + BEEP_DELAY;  
  break;
    
  case 2:
    
    break;
    
    
  }
}
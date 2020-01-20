#include "stm8s_conf.h"
//#include "main.h"
//#include "stdio.h"
//#include "string.h"

#include "Disp_7seg_555.h"

uint16_t u16DispBuffer[3];
uint8_t u8DispBuffDigit[3];


uint8_t u8Dots = 0; // b0 -> dots in AN0 | b1 -> dots in AN1 | b2 -> dots in AN2
uint16_t u16DisplayLeds = 0; 


uint8_t u8DispCounter = 0;


uint8_t CharSet[16] = {
  0x28, //0
  0xEB, //1
  0x32, //2
  0xA2, //3
  0xE1, //4
  0xA4, //5
  0x24, //6
  0xEA, //7
  0x20, //8
  0xA0, //9
  0x00, //0
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};


void _delay(void)
{
  for(uint8_t i = 0; i < 0xFF; i++)nop();
}


void DisplayInit(void)
{
  GPIO_Init(REGISTER_DS_PORT, REGISTER_DS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST); 
  GPIO_Init(REGISTER_ST_PORT, REGISTER_ST_PIN, GPIO_MODE_OUT_PP_HIGH_FAST); 
  GPIO_Init(REGISTER_SH_PORT, REGISTER_SH_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  
  REGISTER_DS_HIGH;
  REGISTER_ST_LOW;
  REGISTER_SH_LOW;
  
  u8DispBuffDigit[0] = CharSet[0];
  u8DispBuffDigit[1] = CharSet[0];
  u8DispBuffDigit[2] = CharSet[0];
}

void Register(uint16_t u16Data)
{
  uint16_t data = u16Data;
  REGISTER_DS_LOW;
  REGISTER_ST_LOW;
  REGISTER_SH_LOW;  
  
  for(uint16_t i = 0; i < 16; i++)
  {
    if(data & 0x0001)REGISTER_DS_HIGH;
    else REGISTER_DS_LOW;
    nop();
    
    data = data >> 1;
    
    REGISTER_SH_HIGH;
    nop();
    REGISTER_SH_LOW;
    nop();
  }
  
  REGISTER_ST_HIGH;
  nop();
  REGISTER_ST_LOW;
  nop();
  
}

void DisplayLed(Leds LedN, LedStatus lStatus)
{
   if(lStatus == ON)
    {
      u16DisplayLeds |= (uint16_t)(1 << LedN);
    }else
    {
      u16DisplayLeds &= (uint16_t)(~(1 << LedN));
    }
}

void DiaplayCalculate(void)
{

  
  for(uint8_t i = 0; i < 3; i++)
  {
    //выключить аноды
    u16DisplayLeds |= (uint16_t)(1 << DISP_AN0);
    u16DisplayLeds |= (uint16_t)(1 << DISP_AN1);
    u16DisplayLeds |= (uint16_t)(1 << DISP_AN2);
    
    u16DispBuffer[i] = u16DisplayLeds;
    
    switch (i)
    {
    case 0:
      u16DispBuffer[i] &= (uint16_t)(~(1 << DISP_AN0));
      break;
      
    case 1:
      u16DispBuffer[i] &= (uint16_t)(~(1 << DISP_AN1));
      break;
      
    case 2:
      u16DispBuffer[i] &= (uint16_t)(~(1 << DISP_AN2));
      break;
    }
    

    u16DispBuffer[i] &= 0xFF00;
    u16DispBuffer[i] += u8DispBuffDigit[i];
    if(u8Dots & (1 << i)) u16DispBuffer[i] &= ((uint16_t)(~(1 << DISP_DOT)));
  }
  




}

void DisplayTick(void)
{
  Register(u16DispBuffer[u8DispCounter]);
  u8DispCounter++;
  if(u8DispCounter > 2)u8DispCounter = 0;


}

void DisplaySetDots(uint8_t Dots)
{
u8Dots = Dots;

}


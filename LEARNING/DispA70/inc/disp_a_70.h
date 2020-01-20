#ifndef __DISP_A_70_H
#define __DISP_A_70_H

/*
#define A70_RST_L		GPIO_WriteLow(A70_RST_PORT, A70_RST_PIN)	
#define A70_RST_H		GPIO_WriteHigh(A70_RST_PORT, A70_RST_PIN)

#define A70_DC_L		GPIO_WriteLow(A70_DC_PORT, A70_DC_PIN)	
#define A70_DC_H		GPIO_WriteHigh(A70_DC_PORT, A70_DC_PIN)

#define A70_SCE_L		GPIO_WriteLow(A70_SCE_PORT, A70_SCE_PIN)	
#define A70_SCE_H		GPIO_WriteHigh(A70_SCE_PORT, A70_SCE_PIN)
*/

 
#ifndef A70_RST_PIN
#define A70_RST_PIN GPIO_PIN_3
#endif

#ifndef A70_RST_PORT
#define A70_RST_PORT GPIOC
#endif

#ifndef A70_DC_PIN
#define A70_DC_PIN GPIO_PIN_4
#endif

#ifndef A70_DC_PORT
#define A70_DC_PORT GPIOC
#endif

#ifndef A70_SCE_PIN
#define A70_SCE_PIN GPIO_PIN_4
#endif

#ifndef A70_SCE_PORT
#define A70_SCE_PORT GPIOD
#endif

#define A70_RST_L	A70_RST_PORT->ODR &= ~A70_RST_PIN 	
#define A70_RST_H	A70_RST_PORT->ODR |= A70_RST_PIN 

#define A70_DC_L	A70_DC_PORT->ODR &= ~A70_DC_PIN	
#define A70_DC_H	A70_DC_PORT->ODR |= A70_DC_PIN

#define A70_SCE_L	A70_SCE_PORT->ODR &= ~A70_SCE_PIN	
#define A70_SCE_H	A70_SCE_PORT->ODR |= A70_SCE_PIN





void LcdClear (void);
void LcdSend (unsigned char data, unsigned char cmd);    			//Sends data to display controller
void LcdUpdate (void);   							//Copies the LCD cache into the device RAM
void LcdInit (void);								//Настройка SPI и дисплея
void LcdContrast (unsigned char contrast); 					//contrast -> Contrast value from 0x00 to 0x7F
void LcdMode (unsigned char mode); 						//режимы дисплея: 0 - blank, 1 - all on, 2 - normal, 3 - inverse
void LcdPwrMode (void);								//инвертирует состояние вкл/выкл дисплея
//void LcdChr (char ch);								//Displays a character at current cursor location and increment cursor location
void Lcd_Set_Curr(unsigned char x, unsigned char y);							
void LcdChrBig (int ch, unsigned char x, unsigned char y);
void LcdUpdate(void);
void Reset_LCD(void);
void LcdDigit(uint8_t u8Digit, uint8_t u8XPos, uint8_t u8YPos);
void ShowSin(void);


#endif




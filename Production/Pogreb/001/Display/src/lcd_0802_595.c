
#include "main.h" //??????????????????????wtf!!
#include "lcd_0802_595.h"
#include "function.h"

LCD_BL_Pin BLight = LCD_BL_LOW;


//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void _Register_595(uint8_t u8Data, LCD_E_Pin E_Pin, LCD_RS_Pin RS_Pin, LCD_BL_Pin BL_Pin)
{

	/*
	b0	RS
	b1	RW
	b2	E
	b3	d4
	b4	d5
	b5	d6
	b6	d7
	b7	BL
	*/
	u8Data = (u8Data & 0x0F) << 3; 
	
	if(E_Pin)u8Data |= (1 << 2);
	else u8Data &= ~(1 << 2);

	if(RS_Pin)u8Data |= (1 << 0);
	else u8Data &= ~(1 << 0);

	if(BL_Pin)u8Data |= (1 << 7);
	else u8Data &= ~(1 << 7);	
	
	for(uint8_t i=0; i<8; i++)
	{
		if((u8Data << i) & 0x80) // ���������� �� �������, ������� �� ��������, �������
		{
			LCD_DATA_HI;
		}
		else
		{
			LCD_DATA_LOW;
		}
		LCD_SCK_HI;
		LCD_SCK_LOW;
	}
	
	LCD_STROBE_HI;
	LCD_STROBE_LOW;
	
	
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void _LCD_Comm(uint8_t u8Comm)
{
	_Register_595(u8Comm, LCD_E_HI, LCD_RS_COMM, BLight);
	_Register_595(u8Comm, LCD_E_LOW, LCD_RS_COMM, BLight);
	Delay_ms(2);
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void _LCD_Data(uint8_t u8Data)
{
	_Register_595(u8Data, LCD_E_HI, LCD_RS_DATA, BLight);
	_Register_595(u8Data, LCD_E_LOW, LCD_RS_DATA, BLight);
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void LCD_Init(uint8_t u8Param)
{
	_Register_595(0x00, LCD_E_LOW, LCD_RS_COMM, BLight);
	
	switch (u8Param)
	{
		case 0: u8Param = 0x0C; break;
		case 1: u8Param = 0x0D; break;
		case 2: u8Param = 0x0E; break;
		case 3: u8Param = 0x0F; break;
	}
	
	Delay_ms(20);			// ����� ��������� ������� ��������� 20 ��
	
	_LCD_Comm(0x03);		// ������� � 4-� ������ �����
	Delay_ms(40);
	_LCD_Comm(0x03);		// ������� � 4-� ������ �����
	Delay_ms(40);
	_LCD_Comm(0x03);		// ������� � 4-� ������ �����
	Delay_ms(40);
	_LCD_Comm(0x02);		// ������� � 4-� ������ �����
	Delay_ms(40);
	_LCD_Comm(0x02);		// ��������� ����������
	_LCD_Comm(0x08);		// ��������� ����������
	_LCD_Comm(0x00);		// ��������� �������
	_LCD_Comm(0x08);		// ��������� �������
	_LCD_Comm(0x00);		// ������� �������
	_LCD_Comm(0x01);		// ������� �������
	_LCD_Comm(0x00);		// ������������� ����� ����� ������
	_LCD_Comm(0x06);		// ������������� ����� ����� ������
	_LCD_Comm(0x00);		// �������� ������� � ��������� ��������
	_LCD_Comm(u8Param);		// �������� ������� � ��������� ��������
	
	
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void LCD_Char(uint8_t u8Char)
{
	_LCD_Data(u8Char >> 4);
	_LCD_Data(u8Char);
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void LCD_Light(LCD_BL_Pin BL)
{
	BLight = BL;
	_Register_595(0x00, LCD_E_LOW, LCD_RS_COMM, BLight);
}


//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void LCD_Clear(void)
{
	_LCD_Comm(0x00);
	_LCD_Comm(0x01);
	Delay_ms(2);
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void LCD_Str(char *str)
{
	while((*str) != '\0')
	{
		LCD_Char(*str);
		str++;
	}
	
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------

void LCD_GotoXY(uint8_t x, uint8_t y)
{
	uint8_t u8Data =  0x80|((x)+((y)*0x40));

	_LCD_Comm(u8Data >> 4);
	_LCD_Comm(u8Data);
	
	Delay_ms(2);						// ����� ��������� �����
}

//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
//*******************************
//------------------------------------------------------------------------------------------------------------












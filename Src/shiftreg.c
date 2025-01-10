/*
 * shiftreg.c
 *
 *  Created on: Dec 15, 2024
 *      Author: wataoxp
 */
#include "shiftreg.h"

static GPIO_TypeDef* LAT_Port;
static GPIO_TypeDef* CLK_Port;
static GPIO_TypeDef* SER_Port;
static uint16_t LAT;
static uint16_t CLK;
static uint16_t SER;

void SetHandle(ShiftLCD_Typedef *init)
{
	//Valid check
	if( (init->PortLAT < 0) || (init->PortLAT < 0) || (init->PortSER < 0)
			|| (init->lat < 0) || (init->lat < 0) || (init->ser < 0))
	{
		while(1);
	}

	LAT_Port = init->PortLAT;
	CLK_Port = init->PortCLK;
	SER_Port = init->PortSER;

	LAT = init->lat;
	CLK = init->clk;
	SER = init->ser;
}
void SetSerial(uint8_t data)
{
	uint8_t i;

	WRITE_REG(LAT_Port->BRR, LAT);
	for(i = 0;i < 8;i++)
	{
		WRITE_REG(CLK_Port->BRR,CLK);

		WRITE_REG(SER_Port->BRR,SER);
		if(data & 0x80)
		{
			WRITE_REG(SER_Port->BSRR,SER);
		}
		data <<= 1;
		WRITE_REG(CLK_Port->BSRR,CLK);
	}
	WRITE_REG(LAT_Port->BSRR,LAT);
	__asm volatile("nop\n");
}
void SetCMD(uint8_t cmd)
{
	uint8_t SetCMD = LCD_RS_CMD | cmd >> 4;	//RS Low(0x00)
	SetSerial(SetCMD | LCD_ENABLE);		//Enable High
	Delay(5);
	SetSerial(SetCMD & ~LCD_ENABLE);		//Hold Data.Enable Low
	Delay(5);

	SetCMD = LCD_RS_CMD | (cmd & 0xf);
	SetSerial(SetCMD | LCD_ENABLE);
	Delay(5);
	SetSerial(SetCMD & ~LCD_ENABLE);
	Delay(5);

	Delay(50);
}
void SetData(uint8_t data)
{
	uint8_t SetData = LCD_RS_DATA | data >> 4;		//RS High(0x40)
	SetSerial(SetData | LCD_ENABLE);		//Enable High
	Delay(5);
	SetSerial(SetData & ~LCD_ENABLE);		//Hold Data.Enable Low
	Delay(5);

	SetData = LCD_RS_DATA | (data & 0xf);
	SetSerial(SetData | LCD_ENABLE);
	Delay(5);
	SetSerial(SetData & ~LCD_ENABLE);
	Delay(5);

	Delay(50);
}
void LCDinit(void)
{
	Init1usTick();

	Delay(6000);
	SetSerial(0x3 | LCD_ENABLE);	//8bit
	Delay(5);
	SetSerial(0x3 & ~LCD_ENABLE);
	Delay(5);
	Delay(5000);

	SetSerial(0x3 | LCD_ENABLE);	//4bit dummy
	Delay(5);
	SetSerial(0x3 & ~LCD_ENABLE);
	Delay(5);
	Delay(100);

	SetSerial(0x3 | LCD_ENABLE);	//4bit dummy
	Delay(5);
	SetSerial(0x3 & ~LCD_ENABLE);
	Delay(5);
	Delay(50);

	SetSerial(0x2 | LCD_ENABLE);	//4bit dummy
	Delay(5);
	SetSerial(0x2 & ~LCD_ENABLE);
	Delay(5);
	Delay(50);

	SetCMD(FUNCTION_SET | TWO_LINE);
	SetCMD(ENTRY_MODE_SET | SHIFT_INCREMENT);
	SetCMD(CLEAR_DISPLAY);
	Delay(3000);
	SetCMD(DISPLAY_ON_OFF | DISPLAY_ON);
}
void SetCusor(uint8_t x,uint8_t y)
{
	uint8_t point = SET_DDRAM | ((y * 0x40) + x);

	SetCMD(point);
	Delay(50);
}
void StringLCD(char *str,uint8_t length)
{
	while(length--)
	{
		SetData(*str++);
	}
}

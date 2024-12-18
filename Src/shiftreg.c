/*
 * shiftreg.c
 *
 *  Created on: Dec 15, 2024
 *      Author: wataoxp
 */
#include "main.h"
#include "shiftreg.h"
#include "HD44780.h"

void Delay_us(uint16_t nTime)
{
	while(nTime--)
	{
		__asm volatile(
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"

				//"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				//"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				//"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
		);
	}
}

void SetSerial(uint8_t data)
{
	uint8_t i;

	WRITE_REG(GPIOB->BRR, LAT_Pin);
	for(i = 0;i < 8;i++)
	{
		WRITE_REG(GPIOA->BRR,CLK_Pin);

		WRITE_REG(GPIOA->BRR,SER_Pin);
		if(data & 0x80)
		{
			WRITE_REG(GPIOA->BSRR,SER_Pin);
		}
		data <<= 1;
		WRITE_REG(GPIOA->BSRR,CLK_Pin);
	}
	WRITE_REG(GPIOB->BSRR,LAT_Pin);
	__asm volatile("nop\n");
}
void SetCMD(uint8_t cmd)
{
	uint8_t SetCMD = LCD_RS_CMD | cmd >> 4;	//RS Low(0x00)
	SetSerial(SetCMD | LCD_ENABLE);		//Enable High
	Delay_us(5);
	SetSerial(SetCMD & ~LCD_ENABLE);		//Hold Data.Enable Low
	Delay_us(5);

	SetCMD = LCD_RS_CMD | (cmd & 0xf);
	SetSerial(SetCMD | LCD_ENABLE);
	Delay_us(5);
	SetSerial(SetCMD & ~LCD_ENABLE);
	Delay_us(5);

	Delay_us(50);
}
void SetData(uint8_t data)
{
	uint8_t SetData = LCD_RS_DATA | data >> 4;		//RS High(0x40)
	SetSerial(SetData | LCD_ENABLE);		//Enable High
	Delay_us(5);
	SetSerial(SetData & ~LCD_ENABLE);		//Hold Data.Enable Low
	Delay_us(5);

	SetData = LCD_RS_DATA | (data & 0xf);
	SetSerial(SetData | LCD_ENABLE);
	Delay_us(5);
	SetSerial(SetData & ~LCD_ENABLE);
	Delay_us(5);

	Delay_us(50);
}
void LCDinit(void)
{
	LL_mDelay(60);
	SetSerial(0x3 | LCD_ENABLE);	//8bit
	Delay_us(5);
	SetSerial(0x3 & ~LCD_ENABLE);
	Delay_us(5);
	LL_mDelay(5);

	SetSerial(0x3 | LCD_ENABLE);	//4bit dummy
	Delay_us(5);
	SetSerial(0x3 & ~LCD_ENABLE);
	Delay_us(5);
	Delay_us(100);

	SetSerial(0x3 | LCD_ENABLE);	//4bit dummy
	Delay_us(5);
	SetSerial(0x3 & ~LCD_ENABLE);
	Delay_us(5);
	Delay_us(50);

	SetSerial(0x2 | LCD_ENABLE);	//4bit dummy
	Delay_us(5);
	SetSerial(0x2 & ~LCD_ENABLE);
	Delay_us(5);
	Delay_us(50);

	SetCMD(FUNCTION_SET | TWO_LINE);
	SetCMD(ENTRY_MODE_SET | SHIFT_INCREMENT);
	SetCMD(CLEAR_DISPLAY);
	LL_mDelay(3);
	SetCMD(DISPLAY_ON_OFF | DISPLAY_ON);
}
void SetCusor(uint8_t x,uint8_t y)
{
	uint8_t point = SET_DDRAM | ((y * 0x40) + x);

	SetCMD(point);
	Delay_us(50);
}
void StringLCD(char *str,uint8_t length)
{
	while(length--)
	{
		SetData(*str++);
	}
}

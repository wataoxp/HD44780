/*
 * HD44780.c
 *
 *  Created on: Nov 26, 2024
 *      Author: wataoxp
 */
#include "main.h"
#include "HD44780.h"

void Delay_us(uint16_t Time)
{
	while(Time--)
	{
		__asm volatile(
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"

				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
				"nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n" "nop \n"
		);
	}
}
void SetCMD(uint8_t CMD)
{
	WRITE_REG(GPIOC->BRR,RS); //RS=0

	__asm volatile("nop\n");	//Enable SetUpTime
	SendCMD(CMD >> 4);
	SendCMD(CMD & 0x0f);
	Delay_us(50);
}
void SetData(uint8_t Asci)
{
	WRITE_REG(GPIOC->BSRR,RS); //RS=18

	__asm volatile("nop\n");	//Enable SetUpTime
	SendData(Asci >> 4);
	SendData(Asci & 0x0f);
	Delay_us(50);
}
void SendCMD(uint8_t Command)
{
	WRITE_REG(GPIOA->BRR,0xFFFF);	//D4~D7 Reset
	//WRITE_REG(GPIOB->BRR,0xFFFF);

	WRITE_REG(GPIOC->BSRR,EN);	//EN=1

	WRITE_REG(GPIOA->BSRR,Command);

//	if(Command & 0x08) WRITE_REG(GPIOA->BSRR,D7);
//	if(Command & 0x04) WRITE_REG(GPIOA->BSRR,D6);
//	if(Command & 0x02) WRITE_REG(GPIOB->BSRR,D5);
//	if(Command & 0x01) WRITE_REG(GPIOA->BSRR,D4);

	Delay_us(5);
	WRITE_REG(GPIOC->BRR,EN);	//EN=0
	Delay_us(5);
}
void SendData(uint8_t data)
{

	WRITE_REG(GPIOA->BRR,0xFFFF);	//D4~D7 Reset
//	WRITE_REG(GPIOB->BRR,0xFFFF);

	WRITE_REG(GPIOC->BSRR,EN);	//EN=1

	WRITE_REG(GPIOA->BSRR,data);

//	if(data & 0x08) WRITE_REG(GPIOA->BSRR,D7);
//	if(data & 0x04) WRITE_REG(GPIOA->BSRR,D6);
//	if(data & 0x02) WRITE_REG(GPIOB->BSRR,D5);
//	if(data & 0x01) WRITE_REG(GPIOA->BSRR,D4);

	Delay_us(5);
	WRITE_REG(GPIOC->BRR,EN);	//EN=0
	Delay_us(5);
}
void InitLCD(void)
{
	/* Initial Setting */

	WRITE_REG(GPIOC->BRR,RS);	//RS=0
	LL_mDelay(60);

	SendCMD(0x3);				//8bit mode
	LL_mDelay(5);

	SendCMD(0x3);				//4bit dummy
	Delay_us(100);

	SendCMD(0x3);				//4bit dummy
	Delay_us(50);

	SendCMD(0x2);				//4bit mode
	Delay_us(50);

	/* Commands */

	SetCMD(FUNCTION_SET | TWO_LINE);	//0x28
	SetCMD(ENTRY_MODE_SET | SHIFT_INCREMENT);	//0x06
	SetCMD(CLEAR_DISPLAY);	//0x01
	LL_mDelay(3);
	SetCMD(DISPLAY_ON_OFF | DISPLAY_ON);	//0x0C
}
void SetCusor(uint8_t x,uint8_t y)
{
	uint8_t point = SET_DDRAM | ((y * 0x40) + x);

	WRITE_REG(GPIOC->BRR,RS); //RS=0

	SendCMD(point >> 4);
	SendCMD(point & 0xf);
	Delay_us(50);
}
void StringLCD(uint8_t *str,uint8_t length)
{
	while(length--)
	{
		SetData(*str++);
	}
}

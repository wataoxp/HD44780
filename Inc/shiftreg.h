/*
 * shiftreg.h
 *
 *  Created on: Dec 15, 2024
 *      Author: wataoxp
 */

#ifndef INC_SHIFTREG_H_
#define INC_SHIFTREG_H_

#include "stm32g0xx.h"
#include "stm32g031xx.h"
#include "delay.h"

#include <stdint.h>

typedef struct{
	GPIO_TypeDef* PortCLK;
	GPIO_TypeDef* PortLAT;
	GPIO_TypeDef* PortSER;
	uint32_t clk;
	uint32_t lat;
	uint32_t ser;
}ShiftLCD_Typedef;

#define LCD_ENABLE 0x10
#define LCD_RS_CMD 0x00
#define LCD_RS_DATA 0x40

/* HD44780 Command */
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02

#define ENTRY_MODE_SET 0x04
#define SHIFT_INCREMENT 0x02
#define SHIFT_ON 0x01

#define DISPLAY_ON_OFF 0x08
#define DISPLAY_ON 0x04
#define CUSOR_ON 0x02
#define BLINK_ON 0x01

#define FUNCTION_SET 0x20
#define BIT4MODE 0x00
#define BIT8MODE 0x10
#define TWO_LINE 0x08
#define DOT5X10 0x04

#define SET_DDRAM 0x80

void SetHandle(ShiftLCD_Typedef *init);
void SetSerial(uint8_t data);
void SetCMD(uint8_t cmd);
void SetData(uint8_t data);
void LCDinit(void);
void SetCusor(uint8_t x,uint8_t y);
void StringLCD(char *str,uint8_t length);

#endif /* INC_SHIFTREG_H_ */

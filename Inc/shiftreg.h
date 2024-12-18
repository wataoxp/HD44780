/*
 * shiftreg.h
 *
 *  Created on: Dec 15, 2024
 *      Author: wataoxp
 */

#ifndef INC_SHIFTREG_H_
#define INC_SHIFTREG_H_

#define LCD_ENABLE 0x10
#define LCD_RS_CMD 0x00
#define LCD_RS_DATA 0x40

void SetSerial(uint8_t data);

void SetCMD(uint8_t cmd);
void SetData(uint8_t data);
void LCDinit(void);
void SetCusor(uint8_t x,uint8_t y);
void StringLCD(char *str,uint8_t length);

#endif /* INC_SHIFTREG_H_ */

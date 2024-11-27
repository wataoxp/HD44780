/*
 * HD44780.h
 *
 *  Created on: Nov 26, 2024
 *      Author: wataoxp
 */

#ifndef INC_HD44780_H_
#define INC_HD44780_H_

#include "main.h"

/* LCD Control Pin */
//#define D4 A0_Pin
//#define D5 A1_Pin
//#define D6 A2_Pin
//#define D7 A3_Pin

#define D4 A7_Pin
#define D5 B0_Pin
#define D6 A11_Pin
#define D7 A12_Pin

#define RS C14_Pin
#define EN C15_Pin

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

void Delay_us(uint16_t Time);
/**
 * microsecond Delay
 * 8nop*7Line=57
 * SystemClock/57≒2us.
 **/
void SendCMD(uint8_t Command);
/**
 * 8Bit Command Write
 **/
void SendData(uint8_t data);
/**
 * 8Bit Data Write
 **/
void SetCMD(uint8_t CMD);
/**
 * 4Bit Command Write
 **/
void SetData(uint8_t Asci);
/**
 * 4Bit Data Write
 **/
void InitLCD(void);
void SetCusor(uint8_t x,uint8_t y);
/**
 * LCD coordinate Set
 * x:min 0 max20
 * y:min 0 max 1
 */

/**
 * ***備考***
 *
 * ***Delayについて***
 * 上記の時間はロジックアナライザで測定しています。
 * かなりの誤差のあるDelayですが、時間が長い分には問題はないと判断しています。
 * 使用される場合はシステムクロックに合わせて調整してください。
 * なお今回は64MHｚ駆動を想定し、64nopを試した結果あまりにも誤差が大きかった為調整しています。
 *
 * ***BSRRとBRR***
 * WRITE_REGでアクセスする2つのレジスタです。
 * BSRRは下位16ビットがそれぞれのピンに対応しており、1をセットすることでHIGHを出力します。
 * BRRは16ビットのみのレジスタで、1をセットすることでLOWを出力します。
 * 今回のソースではGPIOAポートの0～3の4ピンを用いることで4ビットの値をそのまま書き込めるようにしています。
 *
 * 他のポート、ピンを使う際はこのヘッダーファイルに定義しているD4~D7のマクロを使用するピンに合わせて再定義。
 * さらにSend系関数の以下のコメントアウトを解除
 * 	if(Command & 0x08) WRITE_REG(GPIOA->BSRR,D7);
 *	if(Command & 0x04) WRITE_REG(GPIOA->BSRR,D6);
 *	if(Command & 0x02) WRITE_REG(GPIOB->BSRR,D5);
 *	if(Command & 0x01) WRITE_REG(GPIOA->BSRR,D4);
 *	4ビットの引数を各ビットごとにチェックし、ピンを制御します。
 *
 * それと共にWRITE_REG(GPIOA->BSRR,Command);およびWRITE_REG(GPIOA->BSRR,data);をコメントアウトしてください。
 * またGPIOA以外のポートを使う際にはBSRRの引数もGPIOBなどに修正。
 * かつWRITE_REG(GPIOA->BRR,0xFFFF);によるピン出力のリセットもGPIOB等の分も追加するようにしてください。
 *
 * ***Enable SetUpTimeについて***
 * RSの出力変更からEnableを変更するまでの最小時間
 * デバイスによって異なる可能性もあります。三共社のデータシートによるとこの時間は0ns。
 * またRSのホールド時間は10nsとなっています。
 * 64MHｚの場合でも周期は15nsであり、仮に1サイクルであっても時間は足りるはずですが、このDelayは必要でした。
 * LCDによっては不要な可能性もあります。
 **/
#endif /* INC_HD44780_H_ */

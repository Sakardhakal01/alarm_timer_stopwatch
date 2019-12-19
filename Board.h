#ifndef BOARD_H
#define BOARD_H
#include  <msp430xG46x.h>
// Typedefs
typedef unsigned char UInt8;
typedef char          Int8;
typedef unsigned int  UInt16;
typedef int           Int16;
// Port Pins
#define PIN0 BIT0
#define PIN1 BIT1
#define PIN2 BIT2
#define PIN3 BIT3
#define PIN4 BIT4
#define PIN5 BIT5
#define PIN6 BIT6
#define PIN7 BIT7

#define LCD_TEST 1   // Set to 1 to include LCD test functions
#endif // Board_H

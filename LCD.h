#ifndef BOARD_LCD_H
#define BOARD_LCD_H

#include "Board.h"

// LCD Constants
#define LCD_NUM_DIGITS  7                   // Number of digits on LCD
#define LCD_MEM_OFFSET  2                   // Offset from LCDMEM[0]
#define LCD_MEM_LOC     11                  // Num of LCDMEM[] locations used


// LCD Segments
#define LCD_A    BIT0
#define LCD_B    BIT1
#define LCD_C    BIT2
#define LCD_D    BIT3
#define LCD_E    BIT6
#define LCD_F    BIT4
#define LCD_G    BIT5
#define LCD_H    BIT7


// Display Power Level
#define LCD_PWR_LVL_0   0x01
#define LCD_PWR_LVL_1   0x02
#define LCD_PWR_LVL_2   0x04
#define LCD_PWR_LVL_3   0x08
#define LCD_PWR_LVL_4   0x10
#define LCD_PWR_LVL_5   0x20
#define LCD_PWR_OFF     0x40

extern void dispPwrLvl(UInt8 lvl);


// Display Function F1 thru F5
#define LCD_F1          0x01
#define LCD_F2          0x02
#define LCD_F3          0x04
#define LCD_F4          0x08
#define LCD_F5          0x10
#define LCD_FUNC_OFF    0x20

extern void dispFunc(UInt8 mask);


// Display Battery Level
#define LCD_BATT_LOW    0x01
#define LCD_BATT_HALF   0x02
#define LCD_BATT_FULL   0x04
#define LCD_BATT_OFF    0x08

extern void dispBattLvl(UInt8 lvl);


// Display Signal Level
#define LCD_SIG_LVL_0   0x01
#define LCD_SIG_LVL_1   0x02
#define LCD_SIG_LVL_2   0x04
#define LCD_SIG_LVL_3   0x08
#define LCD_SIG_OFF     0x10

extern void dispSigLvl(UInt8 lvl);


// Display Arrow
#define LCD_ARROW_UP     0x01
#define LCD_ARROW_RIGHT  0x02
#define LCD_ARROW_DOWN   0x04
#define LCD_ARROW_LEFT   0x08
#define LCD_ARROW_OFF    0x10

extern void dispArrow(UInt8 mask);


// Display Symbol
#define LCD_SYM_DOL      0x01
#define LCD_SYM_ERR      0x02
#define LCD_SYM_MINUS    0x04
#define LCD_SYM_MEM      0x08
#define LCD_SYM_ENV      0x10
#define LCD_SYM_TX       0x20
#define LCD_SYM_RX       0x40
#define LCD_SYM_OFF      0x80

extern void dispSymbol(UInt8 mask);


// Special LCD characters
#define LCD_DP1          0x0001
#define LCD_DP2          0x0002
#define LCD_COL3         0x0004
#define LCD_DP4          0x0008
#define LCD_COL5         0x0010
#define LCD_DP6          0x0020
#define LCD_DP7          0x0040
#define LCD_8BC          0x0080
#define LCD_SPC_CHAR_OFF 0x0100

extern void dispSpecialChar(UInt16 mask);


//
// Initialize LCD
//
extern void initLCD_A(void);


//
// Clear LCD display
//
extern void clrLCD(void);


//
// Display all segments on LCD
//
extern void dispAllLCDSegs(void);


//
// Display character on LCD
//
//   pos - character position on LCD
//   index - index into LCD_Char_Map[] array
//
extern void dispChar(UInt8 pos, UInt8 index);


extern const UInt8 LCD_MAX_CHARS;


#if LCD_TEST > 0

extern void testAll(void);
extern void testSymbol(void);
extern void testArrow(void);
extern void testFunc(void);
extern void testBatt(void);
extern void testSigLvl(void);
extern void testPwrLvl(void);
extern void testSpecialChar(void);
extern void testChar(void);

#endif // LCD_TEST



#endif // BOARD_LCD_H

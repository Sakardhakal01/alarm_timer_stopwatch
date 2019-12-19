#include "LCD.h"


// LCD Segment Mapping
const UInt8 LCD_Char_Map[] =
{
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F,        // '0' or 'O'
  LCD_B+LCD_C,                                // '1' or 'I'
  LCD_A+LCD_B+LCD_D+LCD_E+LCD_G,              // '2' or 'Z'
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_G,              // '3'
  LCD_B+LCD_C+LCD_F+LCD_G,                    // '4' or 'y'
  LCD_A+LCD_C+LCD_D+LCD_F+LCD_G,              // '5' or 'S'
  LCD_A+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,        // '6' or 'b'
  LCD_A+LCD_B+LCD_C,                          // '7'
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,  // '8' or 'B'
  LCD_A+LCD_B+LCD_C+LCD_F+LCD_G,              // '9' or 'g'
  LCD_A+LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,        // 'A'
  LCD_A+LCD_D+LCD_E+LCD_F,                    // 'C'
  LCD_B+LCD_C+LCD_D+LCD_E+LCD_G,              // 'd'
  LCD_A+LCD_D+LCD_E+LCD_F+LCD_G,              // 'E'
  LCD_A+LCD_E+LCD_F+LCD_G,                    // 'F'
  LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,              // 'H'
  LCD_B+LCD_C+LCD_D+LCD_E,                    // 'J'
  LCD_D+LCD_E+LCD_F,                          // 'L'
  LCD_A+LCD_B+LCD_E+LCD_F+LCD_G,              // 'P'
  LCD_B+LCD_C+LCD_D+LCD_E+LCD_F               // 'U'
};

const UInt8 LCD_MAX_CHARS = (sizeof(LCD_Char_Map)/sizeof(UInt8));


//
// Initialize LCD_A
//
void initLCD_A(void)
{
  // Clear LCD memory
  clrLCD();

  // Configure COM0-COM3 and R03-R33 pins
  P5SEL |= (BIT4 | BIT3 | BIT2);
  P5DIR |= (BIT4 | BIT3 | BIT2);

  // Configure LCD_A
  LCDACTL = LCDFREQ_128 | LCDMX1 | LCDMX0 | LCDSON | LCDON;
  LCDAPCTL0 = LCDS4 | LCDS8 | LCDS12 | LCDS16 | LCDS20 | LCDS24;
  LCDAPCTL1 = 0;
  LCDAVCTL0 = LCDCPEN;
  LCDAVCTL1 = VLCD_2_60;
}


//
// Clear LCD
//
void clrLCD(void)
{
  int i;

  for(i = LCD_MEM_OFFSET; i < (LCD_MEM_OFFSET+LCD_MEM_LOC);  i++)
  {
    LCDMEM[i] = 0;
  }
}


//
// Display all segments on LCD
//
void dispAllLCDSegs(void)
{
  int i;

  for(i = LCD_MEM_OFFSET; i < (LCD_MEM_OFFSET+LCD_MEM_LOC);  i++)
  {
    LCDMEM[i] = 0xff;
  }
}


//
// Display character on LCD
//
void dispChar(UInt8 pos, UInt8 index)
{
  LCDMEM[pos + LCD_MEM_OFFSET] &= ~LCD_Char_Map[8];

  if( pos < LCD_NUM_DIGITS )
  {
    if( index < LCD_MAX_CHARS )
    {
      LCDMEM[pos + LCD_MEM_OFFSET] |= LCD_Char_Map[index];
    }
  }
}


//
// Display power level on LCD
//
void dispPwrLvl(UInt8 lvl)
{
  LCDMEM[7+LCD_MEM_OFFSET] &= ~(BIT1+BIT2+BIT3);
  LCDMEM[8+LCD_MEM_OFFSET] &= ~(BIT0+BIT1+BIT2+BIT3);

  switch(lvl)
  {
  case LCD_PWR_LVL_0:
    LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
    LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0);
    break;
  case LCD_PWR_LVL_1:
    LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
    LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1);
    break;
  case LCD_PWR_LVL_2:
    LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
    LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2);
    break;
  case LCD_PWR_LVL_3:
    LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1);
    LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
    break;
  case LCD_PWR_LVL_4:
    LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1+BIT3);
    LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
    break;
  case LCD_PWR_LVL_5:
    LCDMEM[7+LCD_MEM_OFFSET] |= (BIT1+BIT2+BIT3);
    LCDMEM[8+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
    break;
  };
}


//
// Diplay function F1 thru F5 on LCD
//
void dispFunc(UInt8 mask)
{
  UInt8 tmp;

  LCDMEM[7+LCD_MEM_OFFSET] &= ~0xf1;

  if( mask < LCD_FUNC_OFF )
  {
    tmp = mask << 4;

    if( (mask & LCD_F5) )
    {
      tmp |= BIT0;
    }

    LCDMEM[7+LCD_MEM_OFFSET] |= tmp;
  }
}


//
// Display battery level on LCD
//
void dispBattLvl(UInt8 lvl)
{
  LCDMEM[9+LCD_MEM_OFFSET] &= ~(BIT0+BIT1+BIT2+BIT3);

  switch(lvl)
  {
  case LCD_BATT_LOW:
    LCDMEM[9+LCD_MEM_OFFSET] |= (BIT0+BIT3);
    break;
  case LCD_BATT_HALF:
    LCDMEM[9+LCD_MEM_OFFSET] |= (BIT0+BIT2+BIT3);
    break;
  case LCD_BATT_FULL:
    LCDMEM[9+LCD_MEM_OFFSET] |= (BIT0+BIT1+BIT2+BIT3);
    break;
  };
}


//
// Display signal level on LCD
//
void dispSigLvl(UInt8 lvl)
{
  LCDMEM[9+LCD_MEM_OFFSET] &= ~(BIT4+BIT5+BIT6+BIT7);

  switch(lvl)
  {
  case LCD_SIG_LVL_0:
    LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4);
    break;
  case LCD_SIG_LVL_1:
    LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4+BIT7);
    break;
  case LCD_SIG_LVL_2:
    LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4+BIT6+BIT7);
    break;
  case LCD_SIG_LVL_3:
    LCDMEM[9+LCD_MEM_OFFSET] |= (BIT4+BIT5+BIT6+BIT7);
    break;
  };
}


//
// Display arrow on LCD
//
void dispArrow(UInt8 mask)
{
  LCDMEM[8+LCD_MEM_OFFSET] &= ~(BIT4+BIT5+BIT6+BIT7);

  if( mask < LCD_ARROW_OFF )
  {
    LCDMEM[8+LCD_MEM_OFFSET] |= (mask << 4);
  }
}


//
// Display symbol on LCD
//
void dispSymbol(UInt8 mask)
{
  UInt8 tmp;

  LCDMEM[10+LCD_MEM_OFFSET] &= ~(BIT0+BIT1+BIT2+BIT4+BIT5+BIT6+BIT7);

  if( mask < LCD_SYM_OFF )
  {
    tmp = mask << 4;
    tmp |= mask >> 4;

    LCDMEM[10+LCD_MEM_OFFSET] |= tmp;
  }
}


//
// Display special character on LCD
//
void dispSpecialChar(UInt16 mask)
{
  UInt16 x;
  UInt8 index = 0;

  if( mask & LCD_8BC )
  {
    LCDMEM[10+LCD_MEM_OFFSET] |= BIT3;
  }
  else
  {
    LCDMEM[10+LCD_MEM_OFFSET] &= ~BIT3;
  }

  for(x = 1; x < LCD_8BC; x <<= 1)
  {
    if( x & mask )
    {
      LCDMEM[index+LCD_MEM_OFFSET] |= BIT7;
    }
    else
    {
      LCDMEM[index+LCD_MEM_OFFSET] &= ~(BIT7);
    }

    ++index;
  }
}


#if LCD_TEST > 0

void testChar(void)
{
  static UInt8 pos = 0;
  static UInt8 index = 0;

  dispChar(pos, index);

  if( pos++ >= LCD_NUM_DIGITS )
  {
    pos = 0;

    if( index++ >= LCD_MAX_CHARS )
    {
      index = 0;
    }
  }
}


void testSpecialChar(void)
{
  static UInt16 mask = 1;

  dispSpecialChar(mask);

  if( mask >= LCD_SPC_CHAR_OFF )
  {
    mask = 1;
  }
  else
  {
    mask <<= 1;
  }
}


void testPwrLvl(void)
{
  static UInt8 lvl = LCD_PWR_LVL_0;

  dispPwrLvl(lvl);

  if( lvl >= LCD_PWR_OFF )
  {
    lvl = LCD_PWR_LVL_0;
  }
  else
  {
    lvl <<= 1;
  }
}



void testSigLvl(void)
{
  static UInt8 lvl = LCD_SIG_LVL_0;

  dispSigLvl(lvl);

  if( lvl >= LCD_SIG_OFF )
  {
    lvl = LCD_SIG_LVL_0;
  }
  else
  {
    lvl <<= 1;
  }
}


void testBatt(void)
{
  static UInt8 lvl = LCD_BATT_LOW;

  dispBattLvl(lvl);

  if( lvl >= LCD_BATT_OFF )
  {
    lvl = LCD_BATT_LOW;
  }
  else
  {
    lvl <<= 1;
  }
}


void testFunc(void)
{
  static UInt8 func = LCD_F1;

  dispFunc(func);

  if( func >= LCD_FUNC_OFF )
  {
    func = LCD_F1;
  }
  else
  {
    func <<= 1;
  }
}


void testArrow(void)
{
  static UInt8 dir = LCD_ARROW_UP;

  dispArrow(dir);

  if( dir >= LCD_ARROW_OFF )
  {
    dir = LCD_ARROW_UP;
  }
  else
  {
    dir <<= 1;
  }
}


void testSymbol(void)
{
  static UInt8 sym = LCD_SYM_DOL;

  dispSymbol(sym);

  if( sym >= LCD_SYM_OFF )
  {
    sym = LCD_SYM_DOL;
  }
  else
  {
    sym <<= 1;
  }
}


void testAll(void)
{
  UInt8 x;

  for(x = 0; x < 3; x++)
  {
    _BIS_SR(LPM3_bits + GIE);               // LPM3, enable interrupts

    dispAllLCDSegs();
  }

  clrLCD();
}

#endif // LCD_TEST

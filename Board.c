#include "Board.h"
#include "LCD.h"
#include <time.h>
#include <math.h>

#define SW1_PRESSED ((P1IN & BIT0) == 0)
#define SW2_PRESSED ((P1IN & BIT1) == 0)

#define ALARM_MODE 0
#define STOPWATCH_MODE 1
#define TIMER_MODE 2

#define SELECT_MODE 0
#define USER_TIME_INPUT 1
#define CONTROL_STOPWATCH 2
#define CONTROL_TIMER 3
#define CONTROL_ALARM 4

void initStopwatch(void);
void initTimer(void);
void initAlarm(void);
void initState(void);
void initSwitch(void);

long int digit1 = 0;		//first LCD digit
long int digit2 = 0;		//second LCD digit
long int digit3 = 0;		//third LCD digit
long int digit4 = 0;		//fourth LCD digit
long int digit5 = 0;		//fifth LCD digit
long int digit6 = 0;		//sixth LCD digit
long int toDo = 0;			//determines what switch press does
long int mode = 0;			//alarm, timer or stopwatch

int userTime[6] = {0, 0, 0, 0, 0, 0};	//digits entered by the user as time
long int userTimeSec;					//time entered by the user in Sec
long int alarmCount;					//the alarm count in Sec

int m = 0, n = 6;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;					//stop WDT
  initLCD_A();                              //initialize LCD_A
  initSwitch();								//initialize switch interrupts
  P3SEL |= BIT5;							//P3.5 special function
  //Initialize LCD display digits:
  dispChar(1, 10);							
  dispChar(2, 17);
  dispChar(3, 10);
  dispSpecialChar(LCD_DP6);
  dispChar(6, 1);
  _EINT();									//enable iunterrupts
  for(;;)
  {
    __bis_SR_register(LPM0_bits + GIE);		//enter LPM0 with GIE
    if(mode == ALARM_MODE)
    {
        toDo = USER_TIME_INPUT;
    }
    if(mode == STOPWATCH_MODE)
    {
        toDo = CONTROL_STOPWATCH;
        initStopwatch();
    }
    if(mode == TIMER_MODE)
    {
        toDo = USER_TIME_INPUT;
    }
  }
}

#pragma vector = TIMERB0_VECTOR
__interrupt void timerBISR(void)
{
    digit1++;								//increment digit1 every second 
    dispChar(1, digit1 % 10);				//display digit1 with wraparound after 9
    if(digit1 % 10 == 0)
    {
        digit2++;							//increment digit2 every 10 seconds 
        dispChar(2, digit2 % 6);			//display digit2 with wraparound after 5
        if(digit2 % 6 == 0)
        {
            digit3++;						//increment digit every minute
            dispChar(3, digit3 % 10);		//display digit3 with wraparound after 9
            if(digit3 % 10 == 0)
            {
                digit4++;					//increment digit4 every 10 minute
                dispChar(4, digit4 % 6);	//display digit4 with wraparound after 5
                if(digit4 % 6 == 0)
                {
                    digit5++;					//increment digit5 every hour
                    dispChar(5, digit5 % 10);	//display digit5 with wraparound after 9
                    if(digit5 % 10 == 0)
                    {
                        initState();			//set all digit counters to zero
                        digit6++;				//increment digit6 every 10 hours
                        dispChar(6, digit6 % 6);	//display digit6 with wraparound after 6
                        if(digit6 % 6 == 0)
                        {
                            initState();			
                            TBCCTL0 &= ~CCIE;	//disable timerB intterupt after limit reached
                        }
                    }
                }
            }
        }
    }
}

#pragma vector = TIMERA0_VECTOR
__interrupt void timerAISR(void)
{

    if(userTimeSec <= 0)
    {
        P3DIR ^= BIT5;						//if user time has elspsed play buzzer
    }
    else
    {
        digit1--;							//decrement digit 1 
        dispChar(1, digit1);				//display digit1
        if(digit1 < 0)						
        {
            digit1 = 9;						//set digit1 back to 9
            digit2--;						//decrement digit2 
            dispChar(1, digit1);			//display digit1
            dispChar(2, digit2);			//display digit2
            if(digit2 < 0)
            {
                digit2 = 5;					//set digit2 back to 5
                digit3--;					//decrement digit3
                dispChar(2, digit2);		//display digit2
                dispChar(3, digit3);		//display digit3
                if(digit3 < 0)
                {
                    digit3 = 9;				//set digit3 back to 9
                    digit4--;				//decrement digit4
                    dispChar(3, digit3);	//display digit3
                    dispChar(4, digit4);	//display digit4
                    if(digit4 < 0)
                    {
                        digit4 = 5;			//set digit4 back to 5
                        digit5--;			//decrement digit5
                        dispChar(4, digit4);	//display digit4
                        dispChar(5, digit5);	//display digit5
                        if(digit5 < 0)
                        {
                            digit5 = 9;			//set digit5 back to 9
                            digit6--;			//decrement digit6
                            dispChar(5, digit5);	//display digit5
                            dispChar(6, digit6);	//display digit6
                            if(digit6 < 0)
                            {
                                P3DIR ^= BIT5;		//play buzzer
                            }
                        }
                    }
                }
            }
        }
        userTimeSec--;						//decrement user time
    }
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    if(alarmCount == 0)		
    {
        P3DIR ^= BIT5;						//play buzzer if alarm count has reached zero
    }
    digit1++;								//increment digit1
    dispChar(1, digit1 % 10);			    //display digit1 with wraparound
    if(digit1 % 10 == 0)
    {
        digit2++;							//increment digit2
        dispChar(2, digit2 % 6);			//display digit2 with wraparound
        if(digit2 % 6 == 0)
        {
            digit3++;						//increment digit3
            dispChar(3, digit3 % 10);		//display digit3 with wraparound
            if(digit3 % 10 == 0)
            {
                digit4++;					//increment digit4
                dispChar(4, digit4 % 6);	//display digit4 with wraparound
                if(digit4 % 6 == 0)
                {
                    digit5++;				//increment digit5
                    dispChar(5, digit5 % 10);	//display digit5 with wraparound
					if (digit6 % 2 == 0 & digit5 % 4 == 0 & digit6 != 0 & digit5 != 0)
					{
						initState();		//set all digits back to zero after 23:59:59 
					}
                    if(digit5 % 10 == 0)
                    {
                        digit6++;					//increment digit6
                        dispChar(6, digit6 % 6);	//display digit6 with wraparound
                    }
                }
            }
        }
    }
    alarmCount--;				//decrease alarm count
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISE(void)
{
    if(toDo == SELECT_MODE)			
    {
        __delay_cycles(20000);
        if(SW1_PRESSED)				
        {
            mode++;							
            if(mode % 3 == ALARM_MODE)			
            {
                dispChar(1, 10);
                dispChar(2, 17);
                dispChar(3, 10);
                dispChar(6, 1);
                dispSpecialChar(LCD_DP6);
            }
            if(mode % 3 == STOPWATCH_MODE)
            {
                dispChar(1, 0);
                dispChar(2, 7);
                dispChar(3, 5);
                dispChar(6, 1);
                dispSpecialChar(LCD_DP6);
            }
            if(mode % 3 == TIMER_MODE)
            {
                dispChar(1, 13);
                dispChar(2, 1);
                dispChar(3, 7);
                dispChar(6, 1);
                dispSpecialChar(LCD_DP6);
            }
        }
        if(SW2_PRESSED)							//mode selected
        {	
            mode = mode % 3;
            initState();						//initialize LCD display
            __bic_SR_register_on_exit(LPM0_bits);	//exit LPM0
        }
    }
    else if(toDo == USER_TIME_INPUT)
    {
        static int lastDigit;
        if(n <= 0 & SW1_PRESSED)		//if user has input time
        {
            if(mode == 0)
            {
                initAlarm();			
                toDo = 4;
            }
            if(mode == 2)
            {
                initTimer();
                toDo = 3;
            }
        }
        __delay_cycles(20000);
        if(SW1_PRESSED & n > 0)			//allow only 24 hours input range
        {
            if((m < 9 & n % 2 == 1 & n <= 4) | (m < 6 & n % 2 == 0 & n <= 4))
            {
                m++;
            }
            if(n == 6 & m < 2)
            {
                m++;
                lastDigit = m;
            }
            if(n == 5)
            {
                if(lastDigit == 2 & m < 4 | lastDigit < 2 & m < 9)
                {
                    m++;
                }
            }
            dispChar(n, m % 10);
        }
        __delay_cycles(20000);
        if(SW2_PRESSED & n > 0)
        {
            n--;
            userTime[n] = m % 10;	//fill userTime with the digit input by user
            m = 0;
        }
    }
    else if(toDo == CONTROL_STOPWATCH)
    {
        __delay_cycles(25000);
        if(SW1_PRESSED & SW2_PRESSED)
        {
            WDTCTL = 0;				//reset board
        }
        if(SW2_PRESSED)				//reset display and counters
        {
            digit1 = 0;
            digit2 = 0;
            digit3 = 0;
            digit4 = 0;
            digit5 = 0;
            digit6 = 0;
            initState();
            TB0CCTL0 &= ~CCIE;
        }
        if(SW1_PRESSED)				//pause
        {
            TB0CCTL0 ^= CCIE;
        }
    }
    else if(toDo == CONTROL_TIMER)
    {
        __delay_cycles(25000);
        if(SW1_PRESSED & SW2_PRESSED)	//reset board
        {
            P3DIR &= ~BIT5;
            WDTCTL = 0;
        }
        if(SW2_PRESSED)					//reset display and counter
        {
            toDo = 1;
            digit1 = 0;
            digit2 = 0;
            digit3 = 0;
            digit4 = 0;
            digit5 = 0;
            digit6 = 0;
            initState();
            TACCTL0 &= ~CCIE;
            P3DIR &= ~BIT5;
            toDo = USER_TIME_INPUT;
            m = 0;
            n = 6;
        }
        if(SW1_PRESSED)					//pause
        {
            TACCTL0 ^= CCIE;
            P3DIR &= ~BIT5;
        }
    }
    else if(toDo == CONTROL_ALARM)
    {
        __delay_cycles(20000);
        if(SW1_PRESSED & SW2_PRESSED)		//reset board
        {
            WDTCTL = 0;
        }
        if(SW1_PRESSED)						//stop buzzer
        {
            P3DIR &= ~BIT5;
        }
    }
    else
    {
        initState();
    }
    P1IFG &= ~(BIT0 + BIT1);
}


void initStopwatch(void)
{
    TB0CTL = TBSSEL_1 + MC_1;		//ALCK, UP mode
    TB0CCTL0 = CCIE;				//enable interrupt
    TB0CCR0 = 32767;				//1s interrupt trigger
}

void initTimer(void)
{
    int hours = userTime[4] + 10*userTime[5];	//hours input by user
    int mins = userTime[2] + 10*userTime[3];	//mins input by user
    int secs = userTime[0] + 10*userTime[1];	//secs input by user
    dispChar(1, userTime[0]);
    dispChar(2, userTime[1]);
    dispChar(3, userTime[2]);
    dispChar(4, userTime[3]);
    dispChar(5, userTime[4]);
    dispChar(6, userTime[5]);
    digit1 = userTime[0];
    digit2 = userTime[1];
    digit3 = userTime[2];
    digit4 = userTime[3];
    digit5 = userTime[4];
    digit6 = userTime[5];
    userTimeSec = secs + 60*mins + 60*60*hours;	//calculate usertime in secs
    TB0CTL = TBSSEL_1 + MC_1;					//ACLK, UP mode
    TB0CCTL4 = OUTMOD_4;						//TB0 is in toggle mode
    TBCCR0 = 16;								
    TBCCR4 = 16;
    TACTL = TASSEL_1 + MC_1;					//ACLK, UP mode
    TACCTL0 = CCIE;								//enable interrupt
    TACCR0 = 32767;								//1s interrupt trigger
}

void initState(void)
{
    dispChar(1, 0);
    dispChar(2, 0);
    dispChar(3, 0);
    dispChar(4, 0);
    dispChar(5, 0);
    dispChar(6, 0);
    dispSpecialChar(0x0014);
}


void initSwitch(void)
{
    P1IE |= BIT0 + BIT1;
    P1IES |= BIT0 + BIT1;
    P1IFG &= ~(BIT0 + BIT1);
}

void initAlarm(void)
{
     long int currTime;
     time_t raw_time;
     struct tm* ptr_ts;
     time(&raw_time);
     ptr_ts = gmtime(&raw_time);
     int currHour = abs((ptr_ts->tm_hour)-6) % 24;		//convert GMT to CST
     if(ptr_ts->tm_hour >= 0 & ptr_ts->tm_hour <= 6)
     {
         currHour = 24 - currHour;
     }
     int currMin = ptr_ts->tm_min;
     int currSec = ptr_ts->tm_sec;
     int hours = userTime[4] + 10*userTime[5];	//hours input by user
     int mins = userTime[2] + 10*userTime[3];	//mins input by user
     int secs = userTime[0] + 10*userTime[1];	//secs input by user
     userTimeSec = secs + 60*mins + 60*60*hours;
     currTime = currSec + 60*currMin + 60*60*currHour;
     digit1 = currSec % 10;
     dispChar(1, digit1);
     digit2 = currSec/10;
     dispChar(2, digit2);
     digit3 = currMin % 10;
     dispChar(3, digit3);
     digit4 = currMin/10;
     dispChar(4, digit4);
     digit5 = currHour % 10;
     dispChar(5, digit5);
     digit6 = currHour/10;
     dispChar(6, digit6);
     if((userTimeSec - currTime) >= 0)
     {
         alarmCount = userTimeSec - currTime;	
     }
     else
     {
         alarmCount = 86400 - abs(userTimeSec - currTime);	
     }
     WDTCTL = WDT_ADLY_1000;					//1s interval timer
     IE1 |= WDTIE;								//enable WDT interrupt
     TB0CTL = TBSSEL_1 + MC_1;					//ACLK, UP mode
     TB0CCTL4 = OUTMOD_4;						//output is in toggle mode
     TBCCR0 = 16;						
     TBCCR4 = 16;
}






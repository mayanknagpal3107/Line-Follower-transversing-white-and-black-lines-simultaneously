#include<avr/io.h>
#include<util/delay.h>
#include "uart.h"
#include "uart.c"
#include<avr/interrupt.h>
#include<compat/deprecated.h>
#include<stdlib.h>



#define S0 PA0
#define S1 PA1
#define S2 PA2
#define S3 PA3

volatile int flag=0;
volatile int counter=0;
volatile int countR=0,countG=0,countB=0;
volatile int str[5],buzz[5],str1[5],str2[5];


void setup()
{
uart_init(UART_BAUD_SELECT(9600,F_CPU));
sei();
uart_puts("welcome");
DDRA=0xff; //S0,S1,S2,S3 as output
cbi(PORTA,5);
sei();
}

void interrupt_init()
  {
  //cli();
  GICR=(1<<INT0);
  cbi(DDRD,2);
  MCUCR=0x01; //logic change
  sei();
  }

  void timer1_init(void)
{
  TCCR1A=0x00;
  TCCR1B=0x04; //the clock frequency source 256 points
  TCNT1L=0x87;    //10 ms overflow again
  TCNT1H=0xff;
  TIMSK |= (1<<TOIE1); //allow interrupt
}

void TCS()   

{
	sbi(PORTA,0);		//S0=0 S1=1
	cbi(PORTA,1);       //20% frequency scaling
 //PORTA |=(0<<S0)|(1<<S1)
 int flag=0;
 interrupt_init();
  //attachInterrupt(0, ISR_INTO, CHANGE);  
  timer1_init();
}

ISR (INT0_vect)
{
  counter++;
}

int i=0;

ISR(TIMER1_OVF_vect)//the timer 2, 10ms interrupt overflow again. Internal overflow interrupt executive function
{

sbi(PORTC,0);
TCNT1L=0x87;
TCNT1H=0xff;
flag++;
itoa(flag,buzz,10);
uart_puts(buzz);
if(flag==1)
 {
   counter=0;
 }
else if(flag==2)
  {
	cbi(PORTA,2);
	cbi(PORTA,3);
   //PORTA |=(0<<S2)|(0<<S3);
   countR =counter/1.051;
   uart_puts("red=");
   itoa(countR,str,10);
   uart_puts(str);
   sbi(PORTA,2);
   sbi(PORTA,3);
   //PORTA |=(1<<S2)|(1<<S3);   
  }
else if(flag==3)
   {
    countG=counter/1.0157;
	uart_puts("green=");
	itoa(countG,str1,10);
	uart_puts(str1);
	uart_puts("\n\r");
	cbi(PORTA,2);
	sbi(PORTA,3);
	//PORTA |=(0<<S2)|(1<<S3);   
    }
else if(flag==4)
   {
    countB=counter/1.114;
	uart_puts("blue=");
	itoa(countB,str2,10);
	uart_puts(str2);
	uart_puts("\n\r");
	cbi(PORTA,2);
	cbi(PORTA,3);
	//PORTA |=(0<<2)|(0<<3);   
    }
else
    {
    flag=0; 
     TIMSK = 0x00;
    }
    counter=0;
    _delay_ms(2);
}

void loop()
{
 _delay_ms(10);
 
 TCS();
 /*uart_puts("you are find ");
 if((countR>10)||(countG>10)||(countB>10))
  {
     if((countR>countG)&&(countR>countB))
      {
           uart_puts("red");
           uart_puts("\n\r");
           _delay_ms(500);
      }
     else if((countG>=countR)&&(countG>countB))
      {
           uart_puts("green");
           uart_puts("\n\r");
          _delay_ms(500);
      } 
    else if((countB>countG)&&(countB>countR))
     {
           uart_puts("blue");
           uart_puts("\n\r");
          _delay_ms(500);
     }
   }
 else 
 {
    
	uart_puts("NO FIND ");
	_delay_ms(1000); 
 }*/
 	_delay_ms(500); 

} 
 void main()
 {
	 //int s0=3,s1=4,s2=5,s3=6;
	
	setup();
		while(1)
		{
		loop();
		}
 
 }
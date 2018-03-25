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

volatile int flag=0,flag_count=0;
volatile int counter=0;
volatile int countR=0,countG=0,countB=0,countW=0;
volatile char str[10]={0},str1[10]={0},str2[10]={0},buff[10]={0};


void setup()
{
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	sei();
	uart_puts("start");	
	DDRC=0xff;
	sbi(DDRA,0);  //s0
	sbi(DDRA,1);  //s1
	sbi(DDRA,2);  //s2
	sbi(DDRA,3);  //s3
	sbi(DDRA,5);  //OUTPUT ENABLE
	
	cbi(DDRD,2);  //external interrupt

	sbi(PORTA,0); //S0=0 S1=1
	cbi(PORTA,1);  //2% frequency scaling
	
	cbi(PORTA,5); //enable to low	
}
 
 void interrupt_init()
{
	cli();	  
	GICR=(1<<INT0);	 
	MCUCR=0x01; //logic change
	sei();
	//uart_puts("\n ext. int_init");
}
void timer0_init(void)
{
	  TCCR0= (1<<CS02);//prescalor 256
	 
	  TCNT0=75; // delay of 4 ms for frequency of 7372800
	  TIMSK |= (1<<TOIE0); //allow interrupt
	  //uart_puts("\n timer0_init");
}


ISR (INT0_vect)
{
	sbi(PORTC,1);
	counter++;
}

ISR(TIMER0_OVF_vect)//the timer 2, 10ms interrupt overflow again. Internal overflow interrupt executive function
{
	
	TCNT0=75;
	flag++;
	//uart_puts("flag");
	itoa(flag,buff,10);
	
		if(flag==1)
		 {
		   counter=0;
		   cbi(PORTA,2);//s2         low
			cbi(PORTA,3);//s3        low       for getting value of red
		 }
		 
		 else if(flag==100)
		{
			countR =counter;
			uart_puts("\n\rred=");
		    itoa(countR,str,10);
		    uart_puts(str);
			
			cbi(PORTA,2);//s2      low
			sbi(PORTA,3); // s3    high     for selecting blue colour
			counter=0;
		}
		 
		 
		 else if(flag==200)
		{
			countB=counter;
			uart_puts("  blue=");
			itoa(countB,str2,10);
			uart_puts(str2);
			
			sbi(PORTA,2);//s2    high
			sbi(PORTA,3);//s3    high      for selecting green colour
			counter=0;
		}
		 
		 
		else if(flag==300)
		{
			
			countG=counter;
			uart_puts("  green=");
			itoa(countG,str1,10);
			uart_puts(str1);
		    
		    sbi(PORTA,2);//s2   high 
		    cbi(PORTA,3);//s3   low        for selecting no filter 
			counter=0;
			
			//TIMSK = 0x00;
		}
		
		else if(flag==400)
		{
			flag_count=1;
			
			countW=counter;
			/*uart_puts("  white=");
			itoa(countW,str2,10);
			uart_puts(str2);*/
		    
		  
			counter=0;
			flag=0;
			//TIMSK = 0x00;
		}
		
		
		/*else
			{
			flag=0; 
			// TIMSK = 0x00;
			 PORTC=PORTC^0x01;
			}*/
			//counter=0;
		//	_delay_ms(2);
}


 void main()
 {
	setup();
	_delay_ms(500);
	interrupt_init();
	timer0_init();
	
	while(1)
	{						
			
			if(flag_count==1)
		{
		 
			if((countR>countG)&&(countR>countB))
				{
					uart_puts("\n\rfind_red");  
				}
				else 
				if((countG>countR)&&(countG>countB))
				{
				  uart_puts("\n\r find_green");	 
				} 
				else 
				if((countB>countG)&&(countB>countR))
				{
             	   uart_puts("\n\r find_blue");		 
				}
			    else 
			    {
					
					uart_puts("NO FIND ");
				}
				flag_count=0;
		}
	}
	
 
 }
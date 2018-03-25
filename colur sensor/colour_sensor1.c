#include<avr/io.h>
#include<util/delay.h>
#include "uart.h"
#include "uart.c"
#include<avr/interrupt.h>
#include<compat/deprecated.h>
#include<stdlib.h>



#define S0 PB0
#define S1 PB1
#define S2 PB2
#define S3 PB3

volatile int flag=0,flag_count=0,z=0;
volatile int counter=0;
volatile int countR=0,countG=0,countB=0,countW=0;
volatile char str[10]={0},str1[10]={0},str2[10]={0},buff[10]={0};


void setup()
{
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	sei();
	uart_puts("start");	
	DDRC=0xff;
	sbi(DDRB,0);  //s0
	sbi(DDRB,1);  //s1
	sbi(DDRB,2);  //s2
	sbi(DDRB,3);  //s3
	sbi(DDRB,5);  //OUTPUT ENABLE
	
	cbi(DDRD,2);  //external interrupt

	sbi(PORTB,0); //S0=0 S1=1
	cbi(PORTB,1);  //2% frequency scaling
	
	cbi(PORTB,5); //enable to low	
}
 
 void interrupt_init()
{
	cli();	  
	GICR|=(1<<INT0);	 
	MCUCR=0x01; //logic change
	sei();
	uart_puts("\n ext. int_init");
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
	//uart_puts("\nint0");
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
		   cbi(PORTB,2);//s2         low
			cbi(PORTB,3);//s3        low       for getting value of red
		 }
		 
		 else if(flag==28)
		{
			countR =counter;
			uart_puts("\n\rred=");
		    itoa(countR,str,10);
		    uart_puts(str);
			
			cbi(PORTB,2);//s2      low
			sbi(PORTB,3); // s3    high     for selecting blue colour
			counter=0;
		}
		 
		 
		 else if(flag==64)
		{
			countB=counter;
			uart_puts("  blue=");
			itoa(countB,str2,10);
			uart_puts(str2);
			
			sbi(PORTB,2);//s2    high
			sbi(PORTB,3);//s3    high      for selecting green colour
			counter=0;
		}
		 
		 
		else if(flag==101)
		{
			
			countG=counter;
			uart_puts("  green=");
			itoa(countG,str1,10);
			uart_puts(str1);
		    
		    sbi(PORTB,2);//s2   high 
		    cbi(PORTB,3);//s3   low        for selecting no filter 
			counter=0;
			
			//TIMSK = 0x00;
		}
		
		else if(flag==200)
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
	sbi(PORTC,0);
	sbi(PORTC,1);
	sbi(PORTC,2);
	sbi(PORTC,3);
	_delay_ms(2000);
	cbi(PORTC,0);
	cbi(PORTC,1);
	cbi(PORTC,2);
	cbi(PORTC,3);
	
	while(1)
	{						
			
			if(flag_count==1)
		{
			if((countB<1700)&&(countG<1700)&&(countR<1700)) 
			    {
					
					uart_puts("NO FIND ");
				}
				else
				
			if((countR>countG)&&(countR>countB))
				{
					uart_puts("\n\rfind_red"); 
					z=1;
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
			   
				flag_count=0;
		}
		if(z==1)
		{cli();
			sbi(PORTC,0);
			sbi(PORTC,1);
			sbi(PORTC,2);
			sbi(PORTC,3);
		}
	}
	
 }
 
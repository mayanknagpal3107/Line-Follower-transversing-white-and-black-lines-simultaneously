#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "uart.h"
#include "uart.c"
#include <avr/interrupt.h>
volatile int r=0,g=0,b=0;
 volatile char str1[2],str2[2],str3[2];

unsigned int frequency_measure()
{

	int t,i=0;
	//unsigned long z; 
	float x,z;
	
	TCCR1A=0x00;
	TCCR1B= 0x02;  //ICP1 EDGE select and clock set
	_delay_ms(30);

///first setr TCNT0 to 0 to start on first pulse///


while(!(TIFR&(1<<ICF1)));   //input capture flag set??

TCNT1=0;
TIFR=(1<<ICF1);    //always clear flag

//now capture stART//

while(!(TIFR&(1<<ICF1)));

t=ICR1;  //STARTING VALUE TO MEASURE

TIFR=(1<<ICF1);

//now capture ending value to measure period//


while(!(TIFR&(1<<ICF1))); //input capture flag set third time

i=ICR1;
TIFR=(1<<ICF1); 

//always clear flag & doing the substraction get the pulse width//

x=i-t;

z=((float)x*0.542);//1000000;
return ((1/z)*1000000);
}


void measure_RED()
{
	cbi(PORTA,2);
	cbi(PORTA,3);
	uart_puts("red=");
	
	r=(unsigned int)frequency_measure();
	
	
	
	itoa(r,str1,10);
	uart_puts(str1);
	uart_puts("\n\r");
	_delay_ms(500);

	

}

void measure_GREEN()
{
	sbi(PORTA,2);
	sbi(PORTA,3);
	uart_puts("green=");
	
	g=(unsigned int)frequency_measure();
	
	//g=(1/g);
	itoa(g,str2,10);
	uart_puts(str2);
	uart_puts("\n\r");
	_delay_ms(500);	

	
	
}

void measure_BLUE()
{
	cbi(PORTA,2);
	sbi(PORTA,3);
	uart_puts("blue=");
	
	b=(unsigned int)frequency_measure();
	
	//b=1/b;
	itoa(b,str2,10);
	uart_puts(str2);
	uart_puts("\n\r");
	_delay_ms(500);	
	
	
		
}



/*void freq_RED()
{
r=frequency_measure();

itoa(r,str1,10);
uart_puts(str1);
uart_puts("\n\r");
_delay_ms(500);

while(r<=2000)
		{
		uart_puts("red detect");
		_delay_ms(100);
		//uart_puts("\n\r");
		}	
_delay_ms(500);
	
}
void freq_GREEN()
{
g=frequency_measure();

itoa(g,str2,10);
uart_puts(str2);
uart_puts("\n\r");
_delay_ms(500);	


}

void freq_BLUE()
{
b=frequency_measure();

itoa(b,str3,10);
uart_puts(str3);
uart_puts("\n\r");
_delay_ms(500);	

	
}*/


int main()

{

uart_init(UART_BAUD_SELECT(115200,F_CPU));


	sbi(DDRA,0);
	sbi(DDRA,1);
	sbi(DDRA,2);
	sbi(DDRA,3);
	
while(1)
	{
	cbi(PORTA,0);
	sbi(PORTA,1);  //freqeuncy scaling

	
	}

} 



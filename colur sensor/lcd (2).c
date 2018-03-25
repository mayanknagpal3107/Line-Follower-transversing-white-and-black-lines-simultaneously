#include<avr/io.h>
#include<util/delay.h>
#include"uart.h"
#include"uart.c"
#include<compat/deprecated.h>

#define RS_1 PORTB |= (1<<PB0);
#define RS_0 PORTB &= ~(1<<PB0);
#define RW_0 PORTB &= ~(1<<PB1);




void enable_HtoL()
{
_delay_ms(20);
PORTB |= (1<<PB2);
_delay_ms(5);
PORTB &= ~(1<<PB2);
_delay_ms(5);

}
void lcd_cmd(unsigned char x)
{
unsigned char temp1;
PORTB=0X00;
RS_0;
RW_0;

temp1 = x;
temp1 = temp1&0xF0;
temp1 = temp1>>1;


PORTB |= temp1;
//PORTB =  ((x&0xF0)>>1);
					
enable_HtoL();
  

PORTB=0X00; 
_delay_ms(10); 

temp1 = x;
temp1 = temp1&0x0F;
temp1 = temp1<<3;

PORTB |= temp1;

				
//PORTB = ((x & 0x0f)<<3);
  
  enable_HtoL();


  
  PORTB =0X78;
}

void lcd_data(unsigned char y)
{
 
unsigned char temp;
PORTB=0X00;  
RS_1;
RW_0;

/*temp = y;
temp = temp&0xF0;
temp = temp>>1;

PORTB |= temp;*/

PORTB |= ((y & 0xf0)>>1);
  
    
  enable_HtoL();
  
  
  
  PORTB=0X00;
  _delay_ms(10);
  
  RS_1;
  
 /*temp = y;
temp = temp&0x0F;
temp = temp<<3;

PORTB |=temp;*/
  
  
  
 PORTB |= ((y & 0x0f)<<3); ///oring is must
  
  
  enable_HtoL();
 PORTB=0X78;
}


void lcd_Init()
{

	RS_0;
	RW_0;
	
	//lcd_cmd(0x28);
	
	PORTB=0X18;
	
	enable_HtoL();		
	
	_delay_ms(5);
	
	enable_HtoL();
	
	_delay_ms(5);	
	
	
	enable_HtoL();
	
	_delay_ms(5);
  
	PORTB=0X10;
	
	_delay_ms(5);
	
	enable_HtoL();
	
	
	PORTB=0X78;
	
	
 

}

int main()
{

uart_init(UART_BAUD_SELECT(9600,F_CPU));
	sei();
	uart_puts("initialized");//uart_putc('f');
char a[]="dheeraj laxminarayan";

int i;
DDRB=0XFF;
	
	lcd_Init();
	lcd_cmd(0x28);
	lcd_cmd(0x01);
	
	lcd_cmd(0x0F);
	
	while(1)
	{
		
		for(i=0;i<17;i++)
		{
		lcd_data(a[i]);
		
		_delay_ms(200);
		}
		
		for(i=0;i<=15;i++)
		{
		lcd_cmd(0x1c);
		
		
		_delay_ms(400);
		
		}
		
		
		lcd_cmd(0x01);
		
		/*lcd_data('d');
		
		_delay_ms(200);
		
		lcd_data('h');
		_delay_ms(200);
		
		lcd_data('e');
		_delay_ms(200);
		
		lcd_data('e');
		_delay_ms(200);
		
		lcd_data('r');
		_delay_ms(200);
		
		lcd_data('a');
		_delay_ms(200);
		
		lcd_data('j');
		_delay_ms(200);
		
		_delay_ms(200);*/
		
		/*for(i=0;i<=15;i++)
		{
		lcd_cmd(0x1c);
		
		
		_delay_ms(200);
		
		}*/
		
		//while(1);
	
	}
	
	
	
	

}











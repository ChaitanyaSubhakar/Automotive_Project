#include "lcd_header.h"
void lcd_config(void)
{
	IODIR0|=RS|RW|EN|DATA;	 //configure lcd pins as o/p port
	lcd_cmd(0x38);	   //function set: 8bit data, 2 line, 5*7 pixel
	lcd_cmd(0x0E);	  //display ON, cursor ON
	lcd_cmd(0x01);		//clear the display
	lcd_cmd(0x80);		//cmd to display on first row
}

void lcd_cmd(unsigned char c)
{
	IOCLR0=(0xFF<<15); //clear data lines
	IOSET0=(c<<15); //load cmd to data lines
	IOCLR0=RS;		//RS=0 for cmd mode
	IOCLR0=RW;		//RW=0 for write operation
	IOSET0=EN;	   //EN=1
	delay(10);
	IOCLR0=EN; //EN=0
}

void lcd_data(unsigned char d)
{
	IOCLR0=(0xFF<<15); //clear the data lines
	IOSET0=(d<<15); //load the data on data lines
	IOSET0=RS; //RS=1 for data mode
	IOCLR0=RW; //RW=0 for write operation
	IOSET0=EN;	//EN=1
	delay(10);
	IOCLR0=EN;
}

void lcd_str(unsigned char *ptr)
{
	while(*ptr!='\0')
	{
		lcd_data(*ptr);
		ptr++;
	}
}

void lcd_num(unsigned int num)
{
	if(num)
	{
		lcd_num(num/10);
		lcd_data(num%10+0x30);
	}
}
	
void delay(unsigned int a)
{
	unsigned int i,j;
	for(i=0;i<a;i++)
	for(j=0;j<6000;j++);
}
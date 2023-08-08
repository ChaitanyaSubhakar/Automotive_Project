/*            * * * *     SAFE DRIVING ASSURANCE: LPC2129 SEATBELT & HANDBRAKE VERIFICATON SYSTEM          * * * *                */        

#include<lpc21xx.h>
#include"can_fun.c"
#include"lcd_fun.c"

void congif_LEDs();
void congif_buzzer();
void congif_seatbelt();
void congif_handbrake();

int main()
{
	unsigned int b,c;
	congif_LEDs();
	congif_buzzer();
	congif_seatbelt();
	congif_handbrake();
	lcd_config();
	can_config();
	while(1)
	{
		if(IOPIN0&(1<<4))
		{	
			can_tx(0x1F);
			b=can_rx();
			if(b==0x1F)
			{
				lcd_cmd(0x80);
				lcd_str("MSG ID: ");
				lcd_num(b);
				lcd_cmd(0xC0);
				lcd_str("SB YES");
				delay(1000);
				lcd_cmd(0x01);
			}
		}
		else
		{	
			can_tx(0x10);
			b=can_rx();
			if(b==0x10)
			{
				lcd_cmd(0x80);
				lcd_str("MSG ID: ");
				lcd_num(b);
				lcd_cmd(0xC0);
				lcd_str("SB NO");
				delay(1000);
				lcd_cmd(0x01);
			}
		}
		if(IOPIN0&(1<<6))
		{	
			can_tx(0x2F);
			c=can_rx();
			if(c==0x2F)
			{
				lcd_cmd(0x80);
				lcd_str("MSG ID: ");
				lcd_num(c);
				lcd_cmd(0xC0);
				lcd_str("HB YES");
				delay(1000);
				lcd_cmd(0x01);
			}
		}
		else
		{	
			can_tx(0x20);
			c=can_rx();
			if(c==0x20)
			{
				lcd_cmd(0x80);
				lcd_str("MSG ID: ");
				lcd_num(c);
				lcd_cmd(0xC0);
				lcd_str("HB NO");
				delay(1000);
				lcd_cmd(0x01);
			}
		}
		if((b==0x1F) && (c==0x2F))
		{
			lcd_str("Engine is ON");
			delay(1000);
			lcd_cmd(0x01);
			IOSET1=(0xFF<<17);
			delay(1000);
			IOCLR1=(0xFF<<17);
			delay(1000);
		}
		else
		{
			lcd_str("Engine is OFF");
			delay(1000);
			lcd_cmd(0x01);
			IOCLR1=(0xFF<<17);
			delay(500);
			IOSET0=(1<<11);
			delay(500);
			IOCLR0=(1<<11);
			delay(500);
		}
	}
}
void congif_LEDs()
{
	IODIR1|=(0xFF<<17); //Configure LEDs as output port
}
void congif_buzzer()
{
	IODIR0|=(1<<11);    //Configure Buzzer as output port
}
void congif_seatbelt()
{
	IODIR0&=~(1<<4);   //Configure Seatbelt as input port
}
void congif_handbrake()
{
	IODIR0&=~(1<<6);  //Configure Handbrake as input port
}
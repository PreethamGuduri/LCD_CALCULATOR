#include<LPC21xx.h>


/***  USED FOR DELAY ***/
void delay_ms(int a)
{
	for(a*=12000;a>=0;a--);
}
void delay_s(int a)
{
	for(a*=12000000;a>=0;a--);
}
void delay_us(int a)
{
	for(a*=12;a>=0;a--);
}


/*** Commad for LCD ***/
void cmdlcd(int a)
{
		IOCLR0 = 1 << 16;
	  IOCLR0 = 1 << 17;
		IOPIN0 = ((IOPIN0 &(~(255 << 0))) |  a);
		IOSET0 = 1 << 18;
		delay_us(1);
		IOCLR0 = 1 << 18;
		delay_ms(2);
}

/** WRITING CHARACTER TO LCD **/
void CharLCD(char a)
{
		IOSET0 = 1 << 16;
	  IOCLR0 = 1 << 17;
		IOPIN0 = ((IOPIN0 &(~(255 << 0))) |  a);
		IOSET0 = 1 << 18;
		delay_us(1);
		IOCLR0 = 1 << 18;
		delay_ms(2);

}

/*** WRITING STRING TO LCD ***/
void strlcd(char *a)
{
	char *s;
		s=a;
	while(*s){
		CharLCD(*s++);
		//delay_ms(300);
		//if(s==(a+16))
		//	cmdlcd(0xc0);
	}
}

/*** WRITING NUMBER TO LCD ***/
void num_lcd(int num)
{
	char a[10];
	int i=0;
	if(num==0)
	{
		//display 0 on lcd
		CharLCD('0');
	}
	else
	{
		//extract digit by digit and store it into array
		while(num)
		{
			a[i++] = (num%10)+48;
			num = num/10;
		}
		//display digit by digit on LCD 
		for(--i;i>=0;i--)
			CharLCD(a[i]);	
	}
}

/*** WRITING FLOAT TO LCD ***/
void float_LCD(float fNum,int nDP)
{
	int iNum;
	if(fNum<0.0)
	{
		//display '-' on LCD
		CharLCD('-');		
		//change the sign of the given number
		fNum = -fNum;	
	}
	//assign float number to integer variable	
	iNum = fNum;	
	//display integer value on LCD
	num_lcd(iNum);
	//display '.' on LCD
	CharLCD('.');
	//extract digit by digit after decimal point and display it on LCD up to given no.of digits after decimal point
	while(nDP)
	{
		fNum = (fNum-iNum)*10;
		iNum = fNum;
		CharLCD(iNum+0x30);
		nDP--;
	}
}

/*** INITIALSING LCD ***/
void initlcd()
	{
	IODIR0 = 0xff;
	IODIR0 |= 1 << 16; // rs
	IODIR0 |= 1 << 17; //rw
	IODIR0 |= 1 << 18; //en
	
	delay_ms(15);
	cmdlcd(0x30);
	
	delay_ms(1);
	cmdlcd(0x30);
		
	delay_us(100);
	cmdlcd(0x30);

	cmdlcd(0x38);
		
		
	cmdlcd(0x0c);
		
		
	cmdlcd(0x01);
		
	cmdlcd(0x06);
	}

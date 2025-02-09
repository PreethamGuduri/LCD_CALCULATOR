#include<LPC21xx.h>
#include<string.h>
#include<stdlib.h>
#include "init_lcd.h"

#define rw0 16
#define rw1 17
#define rw2 18
#define rw3 19

#define co0 20
#define co1 21
#define co2 22
#define co3 23


/*** DISPLAYING NUMBER TO LCD ***/
void numlcd(int a) 
{			
    if (a > 9 && a < 100) {
        CharLCD((a / 10) + '0');
        CharLCD((a % 10) + '0');
    } else if (a >= 0 && a <= 9) {
        CharLCD((a % 10) + '0');
    }
}

/*** REVERSING STRING ***/
char *rev(char *s)
{   
		int i;
    int len=strlen(s);
    int x=len-1;
    for( i=0;i<len/2;i++)
    {
        s[i]^=s[x]^=s[i]^=s[x];
        x--;
         
    }
   
    return s;
}

/*** CONVERTING NUMBER TO STRING ***/
char *str(int a,int sign)
{
   
    char s[20];
    char *t;
    int i=0;
    if(a==0)	
    {
        
        s[i]=(a+'0');
        s[1]='\0';
        t=s;
        return t;
    }

    while(a)
    {
       
        s[i++]=((a%10)+'0');
        a/=10;
    }
    s[i]='\0';
    strcpy(s,rev(s));
    if(sign)
    {
    memmove(s+1,s,strlen(s)+1);
    s[0]='-';
    }
    t=s;
		return t;
   
}

/*** FINDING OPERAND ***/
int op(char a)
{
    switch(a)
    {
        case '+':
        case '-':
        case '*':
        case '/':
            return 0;
    }
    return 1;
}

/*** REVERSING NUMBER ***/
int rev_num(int a) 
{
    int s=0;
    while(a)
    {
       s=s*10+(a%10);
       a/=10;
    }
    return s;
}

/*** CONVERTING STRINF TO NUMBER ***/
int str_num(char *a)
{
    int s=0;
    while(*a)
    {
       s=s*10+((*a)-'0');
       a++;
    }
   return s;
}

/*** MAIN ***/
int main()
{
	
	int len,in,op1,op2,res,pos_1,pos_2,inc;	// Used for Grouping
	int c0,c1,c2,c3,co_val,rw_val;	// Keypad Values
	char key;	// Collecting key from keypad
	int hold=0;	// used for clear the display
	int flag_sign,in_tmp,op_flag,flag_sign_1;	// used for test (ex:-1-2);
	int ad=0,switch_temp;

	/*** KEYPAD CHARACTERS ***/
	char arr[4][4]={'7','8','9','/',
								'4','5','6','*',
								'1','2','3','-',
								'C','0','=','+'};
	
	char num[20];			// storing user input
	char dest[20];	// groping and calculating user input
	char str_op[20];		// 
									
	int i=12;
	IODIR0 |= 0xff;
	initlcd(); 					// INITIASLING LCD
	cmdlcd(0x80);
	IODIR1 |= 1 << rw0 | 1 << rw1 | 1<< rw2 | 1<<rw3; // Output Direction for Rows

	while(1)
		{
			start:
	/** KEYPAD DETECTION (BUTTON)**/
			IOCLR1 = 15 << rw0;
		while(1)
		{
			c0 = ((1 << co0)&IOPIN1);
			c1 = ((1 << co1)&IOPIN1);
			c2 = ((1 << co2)&IOPIN1);
			c3 = ((1 << co3)&IOPIN1);
			if((c0 && c1 && c2 && c3)==0)
			break;
		}
		while(1)
			{
				IOCLR1 = 1<<rw0;
				IOSET1 = 1<<rw1 | 1 << rw2 | 1 << rw3;
				c0 = ((1 << co0)&IOPIN1);
				c1 = ((1 << co1)&IOPIN1);
				c2 = ((1 << co2)&IOPIN1);
				c3 = ((1 << co3)&IOPIN1);
				if((c0 && c1 && c2 && c3)==0)
				{
						rw_val=0;
						break;
				}
				
				IOCLR1 = 1<<rw1;
				IOSET1 = 1<<rw0 | 1 << rw2 | 1 << rw3;
				c0 = ((1 << co0)&IOPIN1);
				c1 = ((1 << co1)&IOPIN1);
				c2 = ((1 << co2)&IOPIN1);
				c3 = ((1 << co3)&IOPIN1);
				if((c0 && c1 && c2 && c3)==0)
				{
						rw_val=1;
						break;
				}
				
				IOCLR1 = 1<<rw2;
				IOSET1 = 1<<rw1 | 1 << rw0 | 1 << rw3;
				c0 = ((1 << co0)&IOPIN1);
				c1 = ((1 << co1)&IOPIN1);
				c2 = ((1 << co2)&IOPIN1);
				c3 = ((1 << co3)&IOPIN1);
				if((c0 && c1 && c2 && c3)==0)
				{
						rw_val=2;
						break;
				}
				
				IOCLR1 = 1<<rw3;
				IOSET1 = 1<<rw1 | 1 << rw2 | 1 << rw0;
				c0 = ((1 << co0)&IOPIN1);
				c1 = ((1 << co1)&IOPIN1);
				c2 = ((1 << co2)&IOPIN1);
				c3 = ((1 << co3)&IOPIN1);
				if((c0 && c1 && c2 && c3)==0)
				{
						rw_val=3;
						break;
				}
				
			}		
			/*** COLOUMN DETECTION ***/
				if(((1 << co0)&IOPIN1)==0)
					co_val=0;
				if(((1 << co1)&IOPIN1)==0)
					co_val=1;
				if(((1 << co2)&IOPIN1)==0)
					co_val=2;
				if(((1 << co3)&IOPIN1)==0)
					co_val=3;
			
			
		while(1)
		{
	/*** WAITING FOR BUTTON RELEASE ***/
			c0 = ((1 << co0)&IOPIN1);
			c1 = ((1 << co1)&IOPIN1);
			c2 = ((1 << co2)&IOPIN1);
			c3 = ((1 << co3)&IOPIN1);
			if(rw_val == 3&& c0 == 0)
			{
				hold++;
			}
			if((c0 && c1 && c2 && c3)!=0)
			break;
		}
			key=arr[rw_val][co_val];
			if(rw_val == 3 && co_val  == 0)
		{
			/*** CLEARING DISPLAY AMD ASSINGING ARRAY WITH NULL ***/
			if(hold>=320000)
			{	
				cmdlcd(0x01);
				memset(dest, 0, sizeof(dest));  
				ad=0;
				hold=0;
				goto start;
			}
			else
			{
				memmove(dest+(strlen(dest)-1),dest+strlen(dest),strlen(dest+strlen(dest))+1);
				cmdlcd(0x01);
				strlen(dest);	
				ad--;
			}
			hold=0;
		}
		
		/*** MAKING RESULT AS OP1  ***/
		if(key == '=')
		{	
				cmdlcd(0x01);
				memset(dest, 0, sizeof(dest)); 
				strcpy(dest,num);
				ad=strlen(dest);
		}
		cmdlcd(0x80);
		if(key !='=' && key!= 'C')
		dest[ad++]=key;
		strlcd(dest);
		strcpy(num,dest);
		if(op(num[strlen(num)-1])==0)
		{
			memmove(num+(strlen(num)-1),num+strlen(num),strlen(num+strlen(num))+1);
		}
		if(co_val != 3 )
		{
			/*** GROUPING FOR  MULTICATION AND DIVISION ***/
			for(i=0;num[i];i++)
			{
				 len=strlen(num);
				 switch(num[i])
				 {
						case '*':
										if(num[i]=='*')
										{
												
												in=i-1;
												op1=0;op2=0;
												pos_1=0;pos_2=0; // pos find
												inc=0;
											 
												
												for(;op(num[in]);in--)
												{
														
														if(in<0)
														break;
														str_op[inc++]=num[in];
														pos_1++;
													 
												}
													str_op[inc]='\0';
												if(pos_1>1)
												{
													strcpy(str_op,rev(str_op));
												}
													op1=str_num(str_op);
												 
												for(in=i+1;op(num[in]);in++)
												{
														
														if(in==len)
														break;
														op2=op2*10+(num[in]-'0');
														pos_2++;
												}
												res=op1*op2;
												
												strncpy(num+(i-pos_1),str(res,0),strlen(str(res,0)));
												
												memmove(num+((i-pos_1)+strlen(str(res,0))),num+(i+pos_2+1),strlen(num+(i+pos_2+1))+1);
												
												i=0;
										}// multiplication if
										
										break;
						case '/':
										if(num[i]=='/')
										{		
											in=i-1;
											op1=0;op2=0;
											pos_1=0;pos_2=0;inc=0;
											for(in=i-1;op(num[in]);in--)
											{
													if(in<0)
													break;
													str_op[inc++]=num[in];
													pos_1++;
											}
											str_op[inc]= '\0';
											if(pos_1 > 1)
											{
													strcpy(str_op,rev(str_op));
											}
											op1=str_num(str_op);
											for(in=i+1;op(num[in]);in++)
											{
													if(in == len)
													break;
													op2=op2*10+(num[in]-'0');
													pos_2++;
											}
											
											res=op1/op2;
											strncpy(num+(i-pos_1),str(res,0),strlen(str(res,0)));
											memmove(num+((i-pos_1)+strlen(str(res,0))),num+(i+pos_2+1),strlen(num+(i+pos_2+1))+1);
											i=0;                        
										}// division if
										break;
				 } // switch
			} // for multi - div
	
			switch_temp=0;
			/*** GROUPING FOR ADDING AND SUBTRACTION ***/
			for(i=0;num[i];i++)
			{
				if(switch_temp)
					{
							i=0;
							switch_temp=0;
					}
				 len=strlen(num);
				if(num[i]=='+')
				{
						in=i-1;
						op1=0;op2=0;
						pos_1=0,pos_2=0; // pos find
						inc=0;	
						for(;op(num[in]);in--)
						{
								if(in<0)
								break;
								str_op[inc++]=num[in];
								pos_1++;
						}
								str_op[inc]='\0';
								if(pos_1>1)
								{
									strcpy(str_op,rev(str_op));
								}
							 op1=str_num(str_op);
						for(in=i+1;op(num[in]);in++)
						{
								if(in==len)
								break;
								op2=op2*10+(num[in]-'0');
								pos_2++;
						}
						res=op1+op2;
						strncpy(num+(i-pos_1),str(res,0),strlen(str(res,0)));
						memmove(num+((i-pos_1)+strlen(str(res,0))),num+(i+pos_2+1),strlen(num+(i+pos_2+1))+1);
						switch_temp=1;
				}
					if(num[i]=='-')
					{
						flag_sign_1=0;
						T: len=strlen(num);
						in=i-1;
						op1=0;op2=0;
						pos_1=0,pos_2=0; // pos find
						inc=0;
						op_flag=0;							
						if(i==0)
						{
								for(in_tmp=i+1;in_tmp<len;in_tmp++)
								{
										if(op(num[in_tmp])==0)
										{
												op_flag=1;
												if(num[in_tmp]=='+')
												{
														num[in_tmp]='-';
														memmove(num,num+1,strlen(num+1)+1);
														flag_sign_1=1;
														i=in_tmp-1;
														goto T;
												}
												if(num[in_tmp]== '-')
												{
														num[in_tmp]='+';
														memmove(num,num+1,strlen(num+1)+1);
														flag_sign_1=2;
														i=in_tmp-1;
														goto T;
												}
										}
								}
								if(!op_flag)
									goto E;
						}
						if(flag_sign_1 == 1)
						{
								for(in=0;i<len;in++)
								{
										if(op(num[in])==0)
										{
												in-=1;
												break;
										}
								}
						}
						for(;op(num[in]);in--)
						{
								if(in<0)
								break;
								str_op[inc++]=num[in];
								pos_1++;
							 
						}
						str_op[inc]='\0';
						if(pos_1>1)
						{
							strcpy(str_op,rev(str_op));
						}

						op1=str_num(str_op);
									
						for(in=i+1;op(num[in]);in++)
						{
								
									if(in==len)
								break;
								op2=op2*10+(num[in]-'0');
								pos_2++;
						}
						 if(flag_sign_1 == 0)
						 {
								 if(op1>op2)
								 {
											res=op1-op2;
											flag_sign=0;
								 }
									if(op1<op2)
									{
											res=op2-op1;
											flag_sign=1;
									}
									if(op1==op2)
									{
											res=op1-op2;
									}
						 }
							if(flag_sign_1==1)
							{
								if(op1<op2)
								{
										
										res=op2-op1;
										
										flag_sign=0;
										
								}else
								{
										
									 
										res=op1-op2;
										flag_sign=1;
								}
							}
							if(flag_sign_1==2)
							{
									
									res=op1+op2;
									flag_sign=1;
							}
							strncpy(num+(i-pos_1),str(res,flag_sign),strlen(str(res,flag_sign)));
							memmove(num+((i-pos_1)+strlen(str(res,flag_sign))),num+(i+pos_2+1),strlen(num+(i+pos_2+1))+1);
							switch_temp=1;
							i=0;
					}
			} // for - add sub

			E:
			cmdlcd(0xca);
			strlcd("            ");
			cmdlcd(0xca);
			strlcd(num);
			}	// if
	
	}	//	while
		
	
} // main

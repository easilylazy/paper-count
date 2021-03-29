#include <reg52.h>
# include "include/utils.h"
//#define uchar unsigned char
uchar flag,a,i;



sbit led1=P1^0 ;
sbit watch=P1^2;
sbit input=P3^7;
	
void init(){
 	TMOD=0X25;		 //T1 TIMER T0 COUNT
	TH1=0XFD;
	TL1=0XFD;
	resetT0();
	TR1=1;
	TR0=1;
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;

}



void watchChange(){
	
 	if(input==0){
		 sleep(10);
		 if(input==0){
		  	watch=~watch;
			while(!input); // wait for key 
			watch=~watch;
		 }	
	}

}
void main(void){
	init();
	flag=0;
	while(1){
		led1=0;
		watchChange();
		if (flag==1){
			ES=0;
			processInput(a);
			
			SBUF=a;
			while(!TI);
			TI=0;
			
			ES=1;
			flag=0;

		}

	}
}	
void refresh() interrupt 4{

	RI=0;
	a=SBUF;
	flag=1;
	

	 
}

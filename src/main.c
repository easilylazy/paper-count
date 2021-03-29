#include <reg52.h>

#define uchar unsigned char
uchar flag,a,i;
uchar code table[]="Got it";   
uchar code reset[]="RESET";


sbit led1=P1^0 ;
sbit watch=P1^2;
sbit input=P3^7;
void resetT0(){
	//T0
	TH0=0X00;
	TL0=0X00;
}	
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

void sleep(unsigned int xms){
	unsigned int i,j;
 	for(i=0;i<xms;i++){
 		for(j=0;j<110;j++){
			;
		}
	}
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
			if (a=='r'){				
				for(i=0;i<5;i++){
					SBUF=reset[i];
					while(!TI);
					TI=0;
					
				}
				resetT0();
				
			}  else if (a=='d'){	
				uchar high_byte,low_byte;
				high_byte=TL0%256;
				low_byte=TL0/256;			
				SBUF=high_byte;
				while(!TI);
				TI=0;
				SBUF=low_byte;
				while(!TI);
				TI=0;
				
			}else {
				for(i=0;i<6;i++){
					SBUF=table[i];
					while(!TI);
					TI=0;
				}			
			}
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

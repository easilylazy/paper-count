# include <reg52.h>
# include "include/utils.h"
# include "include/kalman.h" 
# define uint unsigned int
uchar flag,a,i;
unsigned int beat;		//计时器T2溢出次数，当前为50ms一次

unsigned int round; 	 //计数器T0溢出次数，每个周期重新计数

sbit led1=P1^0 ;
sbit watch=P1^2;
sbit input=P3^7;
sbit ticker=P0^0;	
void init(){
 	TMOD=0X25;		 //T1 TIMER T0 COUNT
	TH1=0XFD;
	TL1=0XFD;

	RCAP2H=0X004C;
	RCAP2L=0X0000;
	resetT0();
	TR2=1;
	TR1=1;
	TR0=1;
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;

	ET0=1;		  // T0溢出中断，精确计数次数	
	ET2=1;		  // 允许T2中断，以通过自动重装计时


}



void watchChange(){
	
	int i;
 	if(input==0){
		// sleep(10);
		 //if(input==0){	   
		  	watch=~watch;
			while(!input); // wait for key 

			watch=~watch;
//			for(i=0;i<0;i++){
//						watch=~watch;
//			}
	//	 }	
	}
}
void main(void){
	unsigned int last_beat=0,seconds=0;
	unsigned int cnt_sum;//,foo=0X0001,bar=0X0002;
	unsigned int period =5;
	double freq;

	
	
	init();
	flag=0;
	//test_filter();	
	init_filter();
	while(1){
		//input=~input;
		led1=0;
		watchChange();
		if (flag==1){
			ES=0;
			processInput(a);
			if(a=='p'){
				output_string("TH0: ");
				output_int(TH0);

				output_string("TH0*256: ");
				output_int(TH0*256);

				output_string("TH0<<: ");
				output_int(TH0>>8);

				output_string("TL0: ");
				output_int(TL0);
			}
						
			ES=1;
			flag=0;

		}
		if(last_beat!=beat&&beat%20==1){
			ES=0;
			seconds++;
			if (seconds%(1*period)==1){
				cnt_sum= 256*round+TH0;
				//(TH0<<8)+TL0;
				
				freq=(double)cnt_sum/period;

				output_string("sum: ");
				output_int(cnt_sum);

				output_string("freq: ");
				output_int((int)freq);

				output_string("filter: ");
				output_int((int)getXn());

				output_string("round: ");
				output_int(round);
			
				predict();
				update(freq);
				
				// reset
				resetT0();
				round=0;
			}
			ES=1;
			last_beat=beat;
		}

	}
}	

// T0 溢出， 统计+1
void overflow() interrupt 1{
	output_string(" OVERFLOW ");
	round++;	
	TF1=0;	 
}

void refresh() interrupt 4{

	RI=0;
	a=SBUF;
	flag=1;
		 
}	

void timer_50ms() interrupt 5{

	TF2=0;
	beat++;	 

}

#include"include/utils.h"
#define uchar unsigned char;
void sleep(unsigned int xms){
	unsigned int i,j;
 	for(i=0;i<xms;i++){
 		for(j=0;j<110;j++){
			;
		}
	}
}
void resetT0(){
	//T0
	TH0=0X00;
	TL0=0X00;
}
void processInput(unsigned char a){
unsigned char code table[]="Got it";   
unsigned char code reset[]="RESET";
int i;
			if (a=='r'){				
				for(i=0;i<5;i++){
					SBUF=reset[i];
					while(!TI);
					TI=0;
					
				}
				resetT0();
				
			}  else if (a=='d'){
				output_int(TH0);
				output_int(TL0);	

				
			}else {
				for(i=0;i<6;i++){
					SBUF=table[i];
					while(!TI);
					TI=0;
				}			
			}
}
void output_int(unsigned int num){
					
				SBUF=num;
				while(!TI);
				TI=0;

}
uchar displayNum(int num){
	// 一位一位变
	int i;
	for(i=1;;i*=10){
	 	if(num/i==0){
			break;
		}
	}
	return 'z'; 	
}
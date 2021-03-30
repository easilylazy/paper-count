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
	unsigned char *result_pstr;
	result_pstr=int2string(num);
	for(;*result_pstr!='\0';result_pstr++){
	   
	   SBUF=*result_pstr;
       while(!TI);
	   TI=0;
	}
	SBUF='\n';
   	while(!TI);
   	TI=0;					
				

}
uchar* int2string(int num){
	unsigned char num_str[10],this,last;
	// 一位一位变
	int i,b=0;
	this=num;
	for(i=1;;i*=10){ 		
		last=this/10;
		num_str[b]=this-last*10+48;
		b++;
	 	if(last==0){
			num_str[b]='\0';
			break;
		}
		this=last;
	}
	
	return &num_str; 	
}
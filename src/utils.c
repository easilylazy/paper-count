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
	if (a=='r'){				
		output_string("RESET ");
		resetT0();
		
	}  else if (a=='d'){
		output_int(TH0);
		output_int(TL0);	

		
	}else {
		output_string("got it! ");	
		SBUF=a;
		while(!TI);
		TI=0;		
	}
}
void output_int(unsigned int num){				
	unsigned char *result_pstr;
	result_pstr=int2string(num);
	output_string(result_pstr);
	output_char(' ');					
				

}
unsigned char* int2string(int num){
	unsigned char num_str[10],this,last,temp;
	// 一位一位遍历，现在是倒序
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
	// reverse
	for(i=0;i<b/2;i++){
		temp=num_str[i];
		num_str[i]=num_str[b-i-1];
		num_str[b-i-1]=temp;
	}
	return &num_str; 	
}
void output_char(unsigned char ch){
	SBUF=ch;
	while(!TI);
	TI=0;

}
void output_string(unsigned char* pstr){
	for(;*pstr!='\0';pstr++){
	   
	   SBUF=*pstr;
       while(!TI);
	   TI=0;
	}
	return;
}
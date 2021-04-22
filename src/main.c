# include <reg52.h>
# include "include/utils.h"
# include "include/kalman.h" 
# define uint unsigned int
# define MAX_DEBUG 5
uchar flag,a,i;
unsigned int beat;		//计时器T2溢出次数，当前为50ms一次

unsigned int round; 	 //计数器T0溢出次数，每个周期重新计数
unsigned int table[11];	//存放不同次校正对应的数据
unsigned int paperNum[MAX_DEBUG]; //存放校正时的纸张数
unsigned int frequency[MAX_DEBUG];//存放校正时对应的频率值
sbit led1=P1^0 ;
sbit watch=P1^2;
sbit input=P3^7;
sbit ticker=P0^0;	
sbit debugMode=P3^3;
sbit startMode=P3^2;
sbit testPin=P0^0;

sbit CS0=P2^7;
sbit KEY1=P2^4;		//确定
// sbit KEY2=P2^5;
// sbit KEY3=P2^6;
// sbit KEY4=P2^7;
unsigned int BIAS;
unsigned char code seg[18]={0xc0,0xf9,0xa4,0xb0,
                       0x99,0x92,0x82,0xf8,
					   0x80,0x90,  //0~9
					   0x88,0x83,  // ABCDEF
					   0xc6,0xa1,0x86,0x8e,0x7f,
					   0xff //空					  
					   };

unsigned char an[6]={2,0,2,1,4,3};
float K,b; // paperNum=k*frequency+b
void Display(unsigned char *ptemp)
{
	unsigned char i,j;
	WR=0,RD=0;
	for(i=0;i<0x60;i=i+0x10)
	{
		P2=0xA0+i;
		P0=seg[*(ptemp++)+BIAS];
		for(j=0;j<100;j++);   
		P0=0xFF;
	}
	CS0=0;
}
void displayInt(unsigned char i0,unsigned char i1,unsigned char i2,unsigned char i3,unsigned char i4,unsigned char i5){
	an[0]=i0;
 	an[1]=i1;
 	an[2]=i2;
 	an[3]=i3;
 	an[4]=i4;
 	an[5]=i5;
	Display(an);

}
unsigned char ReadKeys(){
	// unsigned char i;
	// //循环扫描4个按键
	// //P2 0xA0
	// bit key;


	if(KEY1==0){
			an[4]=9;
			sleep(100);
			while (!KEY1);//等待按键释放
			return 0;//返回第几个按键被按下
	}
	return 5;
}
void waitKey(){
				// 	flow={10,17,17,17,0,initValue};
				// Display(flow);
	unsigned char key;

	key=ReadKeys();
	while(key==5){
		Display(an);
		key=ReadKeys();
	}
				// flow={10,17,1,1,1,1};
				// Display(flow);
				// sleep(200);
				// key=ReadKeys();
				// while(key==5){
				// 	key=ReadKeys();
				// }
}
void JudgeKey(unsigned char key){
	switch (key)
	{
	case 0:
		an[1]=0;
		/* code */
		break;
	case 1:
		an[1]=1;
		break;
	
	default:
		//an[1]=8;
		break;
	}
}
void displayDebug(){
 	an[0]=17;
 	an[1]=17;
 	an[2]=17;
 	an[3]=17;
 	an[4]=13;
 	an[5]=11;
}

void displayStart(){
 	an[0]=17;
 	an[1]=17;
 	an[2]=17;
 	an[3]=17;
 	an[4]=11;
 	an[5]=14;
}


enum Mode{
	INIT=0,DEBUG,START,NORMAL
};

enum Mode appMode=INIT;
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

	EX1=1;	// 外部中断1：校正模式
	EX0=1;	// 外部中断0：启动模式

	IT1=0;
	IT0=0;

}

// 记录输出电平变化（即电容转换对应的频率）
void watchChange();


// 测量频率
int countFrequency();



void simpleFit();
void simpleFit(){
    int i;
    for(i=6;i<10;i+=4){
        table[i]=(table[i+2]+table[i-2])/2;
    }

    for(i=1;i<10;i+=2){
        table[i]=(table[i+1]+table[i-1])/2;
    }
}



void main(void){

	int freq;
	unsigned char key;

	init();
	flag=0;
	
	while(1){



		//led
		Display(an);  //数码管显示


		//扫描按键
		key=ReadKeys();
	
		// add 通过不同的按键行使不同的功能

		if (key!=5){
			//通过数码管显示按键
			an[0]=key;
			//JudgeKey(key);
		}





		//input=~input;
		led1=0;
		
		//
		/*
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
			}else if(a=='t'){
			 	output_string(" V ");
				output_int(testPin);
			 	output_string(" mode ");
				output_int(appMode);


			}
						
			ES=1;
			flag=0;

		}
		*/
		if(appMode==DEBUG){
			


			unsigned int initValue;
			unsigned char i; 
		
			displayInt(0,0,0,0,0,0);
			//sleep(2000);	
			waitKey();
				// table[2]=7;
				// table[4]=11;
				// table[8]=16;
				// table[10]=18;
			i=0;
			for(initValue=2;initValue<10;initValue*=2){

				displayInt(10,17,17,17,0,initValue);
				waitKey();
				table[initValue]=countFrequency();
				paperNum[i]=initValue;
				frequency[i]=table[initValue];
				displayInt(10,1,1,1,1,1);
				waitKey();
				i++;
			}
			// 进行拟合、获取各纸张对应函数

			//最小二乘法
			{
				{  
					
					unsigned int t1=0, t2=0, t3=0, t4=0;
					unsigned char totalSize=i;
					
					for(; i>0; i--)  
					{  
						t1 += frequency[i]*frequency[i];  
						t2 += frequency[i];  
						t3 += frequency[i]*paperNum[i];  
						t4 += paperNum[i];  
					}  
					K = (t3*totalSize - t2*t4) / (t1*totalSize - t2*t2);  // 求得β1   
					b = (t1*t4 - t2*t3) / (t1*totalSize - t2*t2);        // 求得β2  
				}
			}
			// simpleFit();
			appMode=NORMAL;
		}

		if(appMode==START){
			freq=countFrequency();
			paperNum[0]=K*freq+b;
			if(paperNum[0]<16){
				displayInt(0,0,0,0,0,paperNum[0]);
			}else{
				displayInt(0,0,0,0,0,0);
			}
			waitKey();

			
			appMode=NORMAL;
		}
		
	}
}
void watchChange(){
	
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

int countFrequency(){
	unsigned int last_beat=0,seconds=0;
	unsigned int cnt_sum;//,foo=0X0001,bar=0X0002;
	unsigned int period =2;
	//double freq;
	unsigned int iteration;
	//test_filter();	
	init_filter();

	for(iteration=0;iteration<6;){
		//一直监测翻转情况
		watchChange();
			if(last_beat!=beat&&beat%20==1){
				
				ES=0;
				
				if (seconds%(1*period)==1){
					// 设定计算的周期到达，增加轮数
					iteration++;

					cnt_sum= 256*round+TH0;
					//(TH0<<8)+TL0;
					
					//freq=cnt_sum;//(double)cnt_sum/period;

					// output_string("sum: ");
					// output_int(cnt_sum);

					// output_string("freq: ");
					// output_int((int)freq);

					// output_string("filter: ");
					// output_int((int)getXn());

					// output_string("round: ");
					// output_int(round);
				
					predict();
					update(cnt_sum);
					
					// reset
					resetT0();
					round=0;
				}
				ES=1;
				seconds++;
				last_beat=beat;
		}

	}


		
	return (int)getXn();

}

// T0 溢出， 统计+1
void overflow() interrupt 1{
	//output_string(" OVERFLOW ");
	round++;	
	TF1=0;	 
}
// INT0下降沿或低电平，启动
void start() interrupt 0{
	displayStart();
	sleep(10);
	while(!INT0);
	//appMode=START;
	an[0]=0;
}
// INT1下降沿或低电平，校正
void debug() interrupt 2{
	displayDebug();
	sleep(100);
	while(!INT1);
	appMode=DEBUG;
	an[0]=0;
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

# include <reg52.h>
# include "include/utils.h"
# include "include/kalman.h" 
# define uint unsigned int
# define MAX_DEBUG 4
uchar a,i;
unsigned int beat;		//计时器T2溢出次数，当前为50ms一次

unsigned int round; 	 //计数器T0溢出次数，每个周期重新计数
// unsigned int table[11];	//存放不同次校正对应的数据
unsigned int paperNum[MAX_DEBUG]; //存放校正时的纸张数
unsigned int frequency[MAX_DEBUG];//存放校正时对应的频率值
//直接把555输入接入计数器T0(P3.4)

sbit CS0=P2^7;	//74LS138锁存器使能端
sbit KEY1=P2^4;		//确定按键
// sbit KEY2=P2^5;
// sbit KEY3=P2^6;
// sbit KEY4=P2^7;
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
		P0=seg[*(ptemp++)];
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
	unsigned char key;

	key=ReadKeys();
	while(key==5){
		Display(an);
		key=ReadKeys();
	}
}
// void JudgeKey(unsigned char key){
// 	switch (key)
// 	{
// 	case 0:
// 		an[1]=0;
// 		/* code */
// 		break;
// 	case 1:
// 		an[1]=1;
// 		break;
	
// 	default:
// 		//an[1]=8;
// 		break;
// 	}
// }
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



// 测量频率
int countFrequency();

void main(void){

	int freq;
	// unsigned char key;

	init();
	
	while(1){
		//led
		Display(an);  //数码管显示

		if(appMode==DEBUG){

			unsigned char initValue;
			unsigned char i; 
		
			i=0;
			for(initValue=2;initValue<10;initValue*=2){

				displayInt(10,17,17,17,0,initValue);
				waitKey();
				// table[initValue]=countFrequency();//4*initValue;//
				paperNum[i]=initValue;
				frequency[i]=countFrequency();//table[initValue];
				i++;
			}
			// 进行拟合、获取各纸张对应函数
			waitKey();
			//最小二乘法
			{
				{  
					
					int t1=0, t2=0, t3=0, t4=0;
					unsigned char totalSize=i;
					
					for(i=0; i<totalSize; i++)  
					{  
						t1 += frequency[i]*frequency[i];  
						t2 += frequency[i];  
						t3 += frequency[i]*paperNum[i];  
						t4 += paperNum[i];  
					}  
					K = (float)(t3*totalSize - t2*t4) / (t1*totalSize - t2*t2);  // 求得β1  
					
					b = (float)(t1*t4 - t2*t3) / (t1*totalSize - t2*t2);        // 求得β2  
					appMode=NORMAL;

					//display
					t1=K*100;
					displayInt(1,17,17,t1/100,(t1/10-t1/100*10),t1%10);
					waitKey();
					if(b<0){
						t1=-b*100;
						displayInt(2,17,15,t1/100,(t1/10-t1/100*10),t1%10);
					}else{
						t1=-b*100;
						displayInt(2,17,15,t1/100,(t1/10-t1/100*10),t1%10);
						waitKey();

					}
				}
			}
			displayInt(0,0,0,0,0,0);
			waitKey();
			// simpleFit();

		}

		if(appMode==START){
			float result;
			freq=countFrequency();

			appMode=NORMAL;


			
			displayInt(6,17,17,freq/100,(freq/10-freq/100*10),freq%10);
			waitKey();

			result=K*freq+b;

			freq=(unsigned int)(result+0.5);

			displayInt(9,17,17,freq/100,(freq/10-freq/100*10),freq%10);
			waitKey();

			displayInt(0,0,0,0,0,0);
			waitKey();
		}
		
	}
}

int countFrequency(){
	unsigned int last_beat=0;
	unsigned int cnt_sum,ratio,stableCnt;//,foo=0X0001,bar=0X0002;

	unsigned int iteration=0;
	//test_filter();	
	init_filter();
	beat=0;
	while(1){

		if(iteration>999){
			//超时FFF
			// displayInt(15,15,15,cnt_sum/100,(cnt_sum/10-TH0/100*10),cnt_sum%10);
			waitKey();
			break;

		}

		//一直监测翻转情况
		// displayInt(iteration,17,17,beat/100,(beat/10-beat/100*10),beat%10);
		displayInt(iteration,16,17,cnt_sum/100,(cnt_sum/10-TH0/100*10),cnt_sum%10);



			if(last_beat!=beat&&beat%5==1){
				
				// ES=0;
				// seconds++;
				last_beat=beat;

				
					// 设定计算的周期到达，增加轮数
					iteration++;
					cnt_sum= 256*round+TH0;				
					predict();
					ratio=update(cnt_sum);

					if(ratio<5){
						//与上一次的错误率减小到一定范围
						//累积到一定次数，则退出
						stableCnt++;
						if(stableCnt>20){
							break;
						}
					}else{
						stableCnt=0;
					}
					
					// reset
					resetT0();
					round=0;
					// seconds=0;
				// }
				// ES=1;
				
		}

		

		

	}

	cnt_sum=(int)getXn();
	displayInt(8,16,16,cnt_sum/100,(cnt_sum/10-cnt_sum/100*10),cnt_sum%10);
	waitKey();
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
	sleep(100);
	while(!INT0);
	appMode=START;
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
}	

void timer_50ms() interrupt 5{

	TF2=0;
	beat++;	 

}

# include <reg52.h>
# include "include/utils.h"
# include "include/kalman.h" 
# define uint unsigned int
# define MAX_DEBUG 4
uchar a,i;
unsigned int beat;		//��ʱ��T2�����������ǰΪ50msһ��

unsigned int round; 	 //������T0���������ÿ���������¼���
// unsigned int table[11];	//��Ų�ͬ��У����Ӧ������
unsigned int paperNum[MAX_DEBUG]; //���У��ʱ��ֽ����
unsigned int frequency[MAX_DEBUG];//���У��ʱ��Ӧ��Ƶ��ֵ
//ֱ�Ӱ�555������������T0(P3.4)

sbit CS0=P2^7;	//74LS138������ʹ�ܶ�
sbit KEY1=P2^4;		//ȷ������
// sbit KEY2=P2^5;
// sbit KEY3=P2^6;
// sbit KEY4=P2^7;
unsigned char code seg[18]={0xc0,0xf9,0xa4,0xb0,
                       0x99,0x92,0x82,0xf8,
					   0x80,0x90,  //0~9
					   0x88,0x83,  // ABCDEF
					   0xc6,0xa1,0x86,0x8e,0x7f,
					   0xff //��					  
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
	// //ѭ��ɨ��4������
	// //P2 0xA0
	// bit key;


	if(KEY1==0){
			an[4]=9;
			sleep(100);
			while (!KEY1);//�ȴ������ͷ�
			return 0;//���صڼ�������������
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

	ET0=1;		  // T0����жϣ���ȷ��������	
	ET2=1;		  // ����T2�жϣ���ͨ���Զ���װ��ʱ

	EX1=1;	// �ⲿ�ж�1��У��ģʽ
	EX0=1;	// �ⲿ�ж�0������ģʽ

	IT1=0;
	IT0=0;

}



// ����Ƶ��
int countFrequency();

void main(void){

	int freq;
	// unsigned char key;

	init();
	
	while(1){
		//led
		Display(an);  //�������ʾ

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
			// ������ϡ���ȡ��ֽ�Ŷ�Ӧ����
			waitKey();
			//��С���˷�
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
					K = (float)(t3*totalSize - t2*t4) / (t1*totalSize - t2*t2);  // ��æ�1  
					
					b = (float)(t1*t4 - t2*t3) / (t1*totalSize - t2*t2);        // ��æ�2  
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
			//��ʱFFF
			// displayInt(15,15,15,cnt_sum/100,(cnt_sum/10-TH0/100*10),cnt_sum%10);
			waitKey();
			break;

		}

		//һֱ��ⷭת���
		// displayInt(iteration,17,17,beat/100,(beat/10-beat/100*10),beat%10);
		displayInt(iteration,16,17,cnt_sum/100,(cnt_sum/10-TH0/100*10),cnt_sum%10);



			if(last_beat!=beat&&beat%5==1){
				
				// ES=0;
				// seconds++;
				last_beat=beat;

				
					// �趨��������ڵ����������
					iteration++;
					cnt_sum= 256*round+TH0;				
					predict();
					ratio=update(cnt_sum);

					if(ratio<5){
						//����һ�εĴ����ʼ�С��һ����Χ
						//�ۻ���һ�����������˳�
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

// T0 ����� ͳ��+1
void overflow() interrupt 1{
	//output_string(" OVERFLOW ");
	round++;	
	TF1=0;	 
}
// INT0�½��ػ�͵�ƽ������
void start() interrupt 0{
	displayStart();
	sleep(100);
	while(!INT0);
	appMode=START;
	an[0]=0;
}
// INT1�½��ػ�͵�ƽ��У��
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

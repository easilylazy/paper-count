// filename: utils.h
#define uchar unsigned char;
uchar displayNum(int num){
	// 一位一位变
	int i;
	for(i=1;;i*=10){
	 	if(num/i==0){
			break;
	}
 	
}
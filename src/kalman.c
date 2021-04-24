#include "include/kalman.h"

float Kn,Pn,Pn_1,Rn,X_n,X_n_1,dif;
void init_filter(){
 	//Kn=0.01;
	Pn=0.01;
	Rn=0.01;
	


}
void predict(){
 	X_n_1=X_n;
	Pn_1=Pn;

}

// 返回更新的比例*100
unsigned int update(float Z){

	Kn=Pn/(Pn+Rn);
	Pn_1=(1-Kn)*Pn;
	dif=Kn*(Z-X_n_1);
    X_n=X_n_1+dif;

	if(X_n==0){
		return 100;
	}
	
	if(dif<0){
		return (unsigned int)(-100*dif/X_n);
	}else{
		return (unsigned int)(100*dif/X_n);
	}

}
float getXn(){
	 return X_n;
}

// void test_filter(){
// 	float measure[5]={3.3,3.2,3.1,2.8,2.7};
// 	int i;
// 	init_filter();
// 	for(i=0;i<5;i++){
// 	 	predict();
// 		update(measure[i]);

// 	}

// }
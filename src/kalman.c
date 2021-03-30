#include "include/kalman.h"

float Kn,Pn,Pn_1,Rn,X_n,X_n_1;
void init_filter(){
 	//Kn=0.01;
	Pn=0.01;
	Rn=0.01;
	X_n=2;


}
void predict(){
 	X_n_1=X_n;
	Pn_1=Pn;

}
void update(float Z){

	Kn=Pn/(Pn+Rn);
	Pn_1=(1-Kn)*Pn;
    X_n=X_n_1+Kn*(Z-X_n_1);
}

void test_filter(){
	float measure[5]={3.3,3.2,3.1,2.8,2.7};
	int i;
	init_filter();
	for(i=0;i<5;i++){
	 	predict();
		update(measure[i]);

	}

}
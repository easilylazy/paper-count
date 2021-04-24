#ifndef _KALMAN_H_
#define _KALMAN_H_
// ��ʼ���˲���
void init_filter();
// ͨ����̬ģ�͹�ʽԤ��
void predict();
// ͨ������Ĳ���ֵ����״̬����
unsigned int update(float Z);
// ����һ�� 
// void test_filter();
float getXn();
#endif

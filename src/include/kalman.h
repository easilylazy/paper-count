#ifndef _KALMAN_H_
#define _KALMAN_H_
// 初始化滤波器
void init_filter();
// 通过动态模型公式预测
void predict();
// 通过输入的测量值进行状态更新
void update(float Z);
// 测试一波
void test_filter();
float getXn();
#endif

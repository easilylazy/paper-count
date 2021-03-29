# paper-count

## 完成阶段一：计数器[basic-count](https://github.com/easilylazy/paper-count/tree/basic-count)

### 实现原理

对P3.7进行触发：接通GND，再断开，类似按键工作

再用P1.3进行记录，将脉冲发送到T0计数器端口P3.4，计数器在一次脉冲+1

### 功能

联合串口输入输出实现功能：

- 输入‘d’：展示现在的计数器数值

- 输入‘r’：重置计数器数值

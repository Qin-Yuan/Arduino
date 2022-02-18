/**************************************************************
 * 编码器驱动头文件 ：encoder_driver.h
************************************************************ */
#ifndef _ENCODER_DRIVER_H_
#define _ENCODER_DRIVER_H_

#define LEFT_A 3      // 左电机A相输出
#define LEFT_B 2      // 左电机B相输出，串口中断0
#define RIGHT_A 5     // 右电机A相输出
#define RIGHT_B 4     // 右电机B相输出，串口中断1
void initEncoders();  // 编码器初始化函数
void leftEncoderEventB();    // 左电机编码器B相串口中断函数
void rightEncoderEventB();   // 右电机编码器B相串口中断函数
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();

#endif

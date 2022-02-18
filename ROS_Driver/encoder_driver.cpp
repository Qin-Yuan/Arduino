/**************************************************************
 * 编码器驱动功能程序：encoder_driver.cpp
*************************************************************/
#include "./PinChangeInt.h"       // 外部中断函数库
#include "./encoder_driver.h"
#include "./commands.h"


// 电机编码器计数变量，使用volatile定义避免串口中断中使用被优化
volatile long left_count = 0L;      
volatile long right_count = 0L;

/**********************************
 * 函数名称：initEncoders()
 * 函数功能：串口中断初始化函数
 **********************************/
void initEncoders() {        //  IO口  中断号
	pinMode(LEFT_A,INPUT);   //   3      
	pinMode(LEFT_B,INPUT);   //   2      0
	pinMode(RIGHT_A,INPUT);  //   5
	pinMode(RIGHT_B,INPUT);  //   4      1
	// 中断0即LEFT_B 2 ; 模式：CHANGE(变化)、RISING(上升沿)、FALLING(下降沿)
	attachPinChangeInterrupt(LEFT_A,leftEncoderEventB,CHANGE);  
  	// 中断1即RIGHT_B 4 ; 模式：CHANGE(变化)、RISING(上升沿)、FALLING(下降沿)
	attachPinChangeInterrupt(RIGHT_A,rightEncoderEventB,CHANGE);
}

/**********************************
 * 函数名称：leftEncoderEventB()
 * 函数功能：左电机串口中断触发函数，
 * 用于左电机编码器触发计数，输入为
 * 编码器B相
 **********************************/
void leftEncoderEventB() {
	// Serial.print("左电机 ");
	// Serial.print(left_count);
	// Serial.print(" ");
	// Serial.print(digitalRead(LEFT_A));
	// Serial.println(digitalRead(LEFT_B));
	// 电平变换触发，即实现二分频计数，高电平变化触发
	if(digitalRead(LEFT_A) == digitalRead(LEFT_B)) {
		left_count++;
	}
	else if(digitalRead(LEFT_A) != digitalRead(LEFT_B)){
		left_count--;
	}  
}

/**********************************
 * 函数名称：rightEncoderEventB()
 * 函数功能：右电机串口中断触发函数，
 * 用于右电机编码器触发计数，输入为
 * 编码器B相
 **********************************/
void rightEncoderEventB() {
	// Serial.print("右电机 ");
	// Serial.print(right_count);
	// Serial.print(" ");
	// Serial.print(digitalRead(RIGHT_A));
	// Serial.println(digitalRead(RIGHT_B));
	// 电平变换触发，即实现二分频计数
	if(digitalRead(RIGHT_A) == digitalRead(RIGHT_B)) {
			right_count--;
	}
	else if(digitalRead(RIGHT_A) != digitalRead(RIGHT_B)){
			right_count++;
	}  
}

/****************************************
 * 函数名称：readEncoder(int i)
 * 函数功能：读取左右电机的编码值
 * 参数说明：
 * 			i 	LEFT(0)/RIGHT(1)左右电机
 ***************************************/
long readEncoder(int i) {
	// 左轮
	if (i == LEFT) {
		return left_count;
	}
	// 右轮
	else {
		return right_count;
	}
}

/****************************************
 * 函数名称：resetEncoder(int i)
 * 函数功能：单电机编码器计数重置函数
 * 参数说明：
 * 			i 	LEFT(0)/RIGHT(1)左右电机
 ***************************************/
void resetEncoder(int i) {
	if (i == LEFT) {
		left_count=0L;
		return;
	} else { 
		right_count=0L;
		return;
	}
}

/****************************************
 * 函数名称：resetEncoders()
 * 函数功能：两个电机编码器全部重置函数
 * 参数说明：
 * 			i 	LEFT(0)/RIGHT(1)左右电机
 ***************************************/
void resetEncoders() {
	resetEncoder(LEFT);
	resetEncoder(RIGHT);
}

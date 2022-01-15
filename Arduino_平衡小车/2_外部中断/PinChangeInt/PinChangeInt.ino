/****************************************************************************
 * Ardino综合项目: Aruino nano平衡小车__2__外部中断
 * Author: Benny
 * 功能：当外部中断IO口电平发生变化时触发中断执行函数
 * 注意：1、由于中断会打断正常代码的运行，因此 ISR 的应该尽可能快地执行完毕。
 *       2、在 ISR 中修改的全局变量要用 volatile 修饰符修饰以防止编译器优化
 *       3、在 ISR 中不能使用其他用中断实现的函数，如 millis() delay() 等，延时可以使用delayMicroseconds()，它不是用中断实现的。
****************************************************************************/

#include "./PinChangeInt.h"       // 外部中断函数库

#define PinA_left 2         // 中断0
#define PinA_right 4        // 中断1

volatile long count_right = 0;  // 使用volatile long类型是为了在外部中断修改脉冲计数值时防止被编译器优化
volatile long count_left = 0;   // 使用volatile long类型是为了在外部中断修改脉冲计数值时防止被编译器优化

void setup() {
    Serial.begin(9600);
	pinMode(PinA_left, INPUT);    // 串口中断为输入模式(左轮测速编码器)
	pinMode(PinA_right, INPUT);	  // 串口中断为输入模式(右轮测试编码器)
    sei();                        // 打开中断
    //cli();                      // 关闭中断
}

void loop() {
    // 主函数中循环检测及叠加脉冲测定小车车速,使用电平改变既进入脉冲叠加增加电机的脉冲数，保证小车的精确度。
	attachPinChangeInterrupt(PinA_left, Code_left, FALLING);    // 中断0即PinA_left 2 ; 模式：CHANGE(变化)、RISING(上升沿)、FALLING(下降沿)
	attachPinChangeInterrupt(PinA_right, Code_right, RISING);   // 中断1即PinA_right 4 ;
}

/**********************************
 * 函数名称：Code_left()
 * 函数功能：左侧电机编码器脉冲计数
 **********************************/
void Code_left() {
	count_left ++;
    Serial.print("count_left: ");
    Serial.print(digitalRead(2));
    Serial.print(" ");
    Serial.println(count_left);
}

/***********************************
 * 函数名称：Code_right()
 * 函数功能：右侧电机编码器脉冲计数
 ***********************************/
void Code_right() {
	count_right ++;
    Serial.print("count_right: ");
    Serial.print(digitalRead(4));
    Serial.print(" ");
    Serial.println(count_right);
}

/***************************************************************
 * 电机驱动功能程序
*************************************************************/
#include "Arduino.h"
#include "./motor_driver.h"
#include "./commands.h"

/**********************************
 * 函数名称：initMotorController()
 * 函数功能：电机驱动初始化，设置各引脚
 * 为输出模式
 **********************************/
void initMotorController() {
    // 芯片使能
    pinMode(STBY,OUTPUT);
    digitalWrite(STBY,HIGH);
    // 左电机
    pinMode(PWMA,OUTPUT);
    analogWrite(PWMA,0);
    pinMode(LEFT_1,OUTPUT);
    pinMode(LEFT_2,OUTPUT);
    // 右电机
    pinMode(PWMB,OUTPUT);
    analogWrite(PWMB,0);
    pinMode(RIGHT_1,OUTPUT);
    pinMode(RIGHT_2,OUTPUT);
}

/**********************************
 * 函数名称：setMotorSpeed(int i, int spd)
 * 函数功能：控制单个电机实现正反转以及调速
 * 参数说明：
 *          i      LEFT/RIGHT,选择控制左、右轮 
 *          spd    PWM调速，-255~255范围
 **********************************/
void setMotorSpeed(int i, int spd) {
    // 正反转标志位，默认0为正转；1为反转
    unsigned char reverse = 0;
    // 反转，将spd取为正数，正反转标志位设为1反转
    if (spd < 0)
    {
        spd = -spd;
        reverse = 1;
    }
    // 超过255设置为255最大
    if (spd > 50) {
        spd = 50;
    }
    // 左轮1调速控制
    if (i == LEFT) {
        // 正转
        if(reverse == 0) {
            digitalWrite(LEFT_1,HIGH);
            digitalWrite(LEFT_2,LOW);
            analogWrite(PWMA,spd);
        }
        // 反转
        else if (reverse == 1) {
            digitalWrite(LEFT_1,LOW);
            digitalWrite(LEFT_2,HIGH);
            analogWrite(PWMA,spd);
        }
    }
    // 右轮调速
    else {
        // 正转
        if(reverse == 0) {
            digitalWrite(RIGHT_2,HIGH);
            digitalWrite(RIGHT_1,LOW);
            analogWrite(PWMB,spd);
        } 
        // 反转
        else if (reverse == 1) {
            digitalWrite(RIGHT_2,LOW);
            digitalWrite(RIGHT_1,HIGH);
            analogWrite(PWMB,spd);
        }
    }
}

/*********************************************************
 * 函数名称：setMotorSpeeds(int leftSpeed, int rightSpeed)
 * 函数功能：控制单个电机实现正反转以及调速
 * 参数说明：
 *          leftSpeed     左轮速度，取值范围-255~255
 *          rightSpeed    右轮速度，取值范围-255~255
 *********************************************************/
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
}

/***************************************************************
 * TB6612电机驱动库文件
*************************************************************/
#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#define STBY    8     // 驱动芯片使能端
#define PWMA    9     // 左电机PWM输出控制脚 
#define LEFT_1  7     // 左电机正极
#define LEFT_2  6     // 左电机负极
#define PWMB    10    // 右电机PWM输出控制脚
#define RIGHT_1 13    // 右电机正极
#define RIGHT_2 12    // 右电机负极

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);

#endif

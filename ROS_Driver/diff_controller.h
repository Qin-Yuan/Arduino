/*********************************************************************
 * ROS驱动板PID控制头文件程序
*********************************************************************/
#ifndef _DIFF_CONTROLLER_H_
#define _DIFF_CONTROLLER_H_

#include "./encoder_driver.h"
#include "./commands.h"
#include "./motor_driver.h"

// 创建结构体变量用于存储左右电机的相关参数
typedef struct {
	double TargetTicksPerFrame;    // 目标速度值
	long Encoder;                  // 编码器计数
	long PrevEnc;                  // 上次编码器计数
	int PrevInput;                 // 上次输入
	int PrevErr;                   // 上次误差
	int ITerm;                     // 累计误差值
	long output;                   // PWM输出值
  int Kp;                        // 20
  int Kd;                        // 12
  int Ki;                        // 0
  int Ko;                        // 50
} SetPointInfo;

SetPointInfo leftPID, rightPID;   // 实例化左右电机结构体变量

// PID各参数值     理想值
int Kp = 23;    // 20
int Kd = 12;    // 12
int Ki = 0;     // 0
int Ko = 50;    // 50

unsigned char moving = 0; 		// 运动标志位 

/**********************************
 * 函数名称：resetPID()
 * 函数功能：重置左右电机的参数值
 **********************************/
void resetPID(){
	// 左轮
	leftPID.TargetTicksPerFrame = 0.0;		// 目标值重置为0
	leftPID.Encoder = readEncoder(LEFT);	// 读取左电机编码计数值
	leftPID.PrevEnc = leftPID.Encoder;		// 上次编码计数参数设置为当前值
	leftPID.output = 0;						// 输出值重置为0
	leftPID.PrevInput = 0;					// 上次输入
	leftPID.ITerm = 0;						// 累计误差
	// 右轮
	rightPID.TargetTicksPerFrame = 0.0;
	rightPID.Encoder = readEncoder(RIGHT);
	rightPID.PrevEnc = rightPID.Encoder;
	rightPID.output = 0;
	rightPID.PrevInput = 0;
	rightPID.ITerm = 0;
  // 左电机 PID各参数值     理想值
  leftPID.Kp = 20;    // 20
  leftPID.Kd = 12;    // 12
  leftPID.Ki = 0;     // 0
  leftPID.Ko = 50;    // 50
  // 右电机 PID各参数值     理想值
  rightPID.Kp = 21;    // 20
  rightPID.Kd = 12;    // 12
  rightPID.Ki = 0;     // 0
  rightPID.Ko = 50;    // 50
}

/****************************************
 * 函数名称：doPID(SetPointInfo * p)
 * 函数功能：PID控制器
 * 参数说明：
 * 			p	左右电机结构体变量指针类型
 ****************************************/
void doPID(SetPointInfo *p) {
	long Perror;
	long output;
	int input;
	//Perror = p->TargetTicksPerFrame - (p->Encoder - p->PrevEnc);
	input = p->Encoder - p->PrevEnc;
	Perror = p->TargetTicksPerFrame - input;
	//output = (p->Kp * Perror + p->Kd * (Perror - p->PrevErr) + p->Ki * p->Ierror) / p->Ko;
	// p->PrevErr = Perror;
	output = (p->Kp * Perror - p->Kd * (input - p->PrevInput) + p->ITerm) / p->Ko;
	p->PrevEnc = p->Encoder;
	output += p->output;
	if (output >= MAX_PWM)
		output = MAX_PWM;
	else if (output <= -MAX_PWM)
		output = -MAX_PWM;
	else
		p->ITerm += p->Ki * Perror;
	p->output = output;
	p->PrevInput = input;
	// PID参数图形调节
	//Serial.println(input);
}

/****************************************
 * 函数名称：updatePID()
 * 函数功能：小车PID调速控制实现
 ****************************************/
void updatePID() {
	leftPID.Encoder = readEncoder(LEFT);    // 读取左轮编码值
	rightPID.Encoder = readEncoder(RIGHT);	// 读取右轮编码值
	// 如果运动标志位为0，则重置左右电机PID输入输出误差等结构体变量值
	if (!moving){
		if (leftPID.PrevInput != 0 || rightPID.PrevInput != 0) {
			resetPID();
		}
		return;
	}
	// 通过PID调速器计算速度控制值
	doPID(&rightPID);
	doPID(&leftPID);
	// 将PID控制器计算的输出值调节小车速度
	setMotorSpeeds(leftPID.output, rightPID.output);
}
#endif

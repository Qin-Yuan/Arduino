/*************************************************************
 * Arduino综合项目: Arduino UNO平衡小车__4__MPU6050陀螺仪
 * Author: Benny
 * 功能：使用Aruino采集陀螺仪的的数据
 * IO接线    MPU6050     Aruino
 *             SCL        A5
 *             SDA        A4
 *             VCC        5V
 *             GND        GND
 * 问题：采集的数据干扰较大，下一节使用卡尔曼滤波处理数据
**************************************************************/
// Github下载链接
// https://github.com/Qin-Yuan/Arduino/tree/main/Arduino_%E5%B9%B3%E8%A1%A1%E5%B0%8F%E8%BD%A6/4_MPU6050%E9%99%80%E8%9E%BA%E4%BB%AA/MPU6050

#include "./I2Cdev.h"             // I2C通信获取MPU6050数据
#include "./MPU6050_6Axis_MotionApps20.h"  // 6050陀螺仪控制库
#include "Wire.h"

MPU6050 mpu ;       // 实例化一个 MPU6050 对象，对象名称为 mpu
int16_t ax, ay, az, gx, gy, gz;  // MPU6050陀螺仪6轴数据参数

//****************** 角度数据集 *******************
float K1 = 0.05;        // 对加速度计取值的权重
float angle0 = 0.00;    // 机械平衡角

void setup() {
	// 加入I2C总线
	Wire.begin();                 // 加入 I2C 总线序列
	Serial.begin(9600);           // 开启串口，设置波特率为 9600
	delay(1500);				  // 延时
	mpu.initialize();             // 初始化MPU6050陀螺仪
	delay(2);				      // 延时
}

void loop() {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);     // IIC获取MPU6050六轴数据 ax ay az gx gy gz
    // 输出陀螺仪的数据
    Serial.print("x轴加速度: "); Serial.println(ax);
    Serial.print("y轴加速度: "); Serial.println(ay);
    Serial.print("z轴加速度: "); Serial.println(az);
    Serial.print("x轴角度: "); Serial.println(gx);
    Serial.print("y轴角度: "); Serial.println(gy);
    Serial.print("z轴角度: "); Serial.println(gz);
    Serial.println();
    delay(1500);
}

/*********************************************************************
 * ROSArduinoBridge
 * 可以通过一组简单的串口命令来控制差分机器人并接收回传的传感器与里程计
 * 数据，ROS驱动板使用的是Atmega328P主控+TB6612电机驱动芯片，如果使用其他的
 * 编码器或电机驱动需要重写readEncoder()与setMotorSpeed()函数
 *********************************************************************/
#include "Arduino.h"
#include "./commands.h"           // 串口指令头文件
#include "./motor_driver.h"       // 电机驱动
#include "./encoder_driver.h"     // 编码器驱动
#include "./diff_controller.h"    // PID调速

// ino中使用的
#define BAUDRATE   57600                        // 波特率
#define PID_RATE   30                           // Hz调速频率
const int PID_INTERVAL = 1000 / PID_RATE;       // 调速周期
unsigned long nextPID = PID_INTERVAL;
#define AUTO_STOP_INTERVAL 300                 // 自动结束时间(可按需修改)
long lastMotorCommand = AUTO_STOP_INTERVAL;

int arg = 0;     // 参数数据标识符；argv1标识符为1；argv2标识符为2
int index = 0;   // 串口数据存储数组索引
char chr;        // 串口数据存储变量
char cmd;        // 存储串口数据的第一个功能指令
char argv1[16];  // 存储数据域的第一个字符串
char argv2[16];  // 存储数据域及后面的字符串
long arg1;       // 用于存储数据域第一个字符串转为int型
long arg2;       // 用于存储数据域第二个及后续的字符串转int型

// 单片机初始化设置函数
void setup() {
    Serial.begin(BAUDRATE);
    //Serial.println("ROS_Arduino驱动板！");
    initEncoders();               // 编码器计数初始化函数
    initMotorController();        // 初始化电机控制
    resetPID();                   // 重置PID参数值
}

// 主程序
void loop() {
    //serial();
    // 间隔适当的时间进行PID控制
    if (millis() > nextPID) {
        updatePID();   // PID调速
        nextPID += PID_INTERVAL;
    }
    // 检查我们是否超过了自动停止间隔
    if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {;
        setMotorSpeeds(0, 0);
        moving = 0;
    }
}

/**********************************
 * 函数名称：resetCommand()
 * 函数功能：重置命令
 **********************************/
void resetCommand() {
    cmd = NULL;
    memset(argv1, 0, sizeof(argv1));    // 初始化值域
    memset(argv2, 0, sizeof(argv2));
    arg1 = 0;
    arg2 = 0;
    arg = 0;
    index = 0;
}

/**********************************
 * 函数名称：resetCommand()
 * 函数功能：执行串口命令
 **********************************/
int runCommand() {
    int i = 0;              // pid_args数组索引计数
    char *p = argv1;        // 指向argv1数组的指针，用于字符串分割传递指针变量
    char *str;              // 用于PID参数设置字符分割存储变量
    int pid_args[4];        // 存储PID四个参数值变量，参数之间用冒号：隔开
    arg1 = atoi(argv1);     // 将字符串转换为整数int型，遇到数字或者正负符号才会转换；其它符号返回0
    arg2 = atoi(argv2);
    // 指令解析
    switch(cmd) {
        // 读取波特率 b
        case GET_BAUDRATE:
            Serial.println(BAUDRATE);
            break;
        // 模拟值读取 a
        case ANALOG_READ:
            Serial.println(analogRead(arg1));
            break;
        // 数字值读取 d
        case DIGITAL_READ:
            Serial.println(digitalRead(arg1));
            break;
        // 模拟写入 x
        case ANALOG_WRITE:
            analogWrite(arg1, arg2);
            Serial.println("OK"); 
            break;
        // 数字写入 w
        case DIGITAL_WRITE:
            if (arg2 == 0) digitalWrite(arg1, LOW);
            else if (arg2 == 1) digitalWrite(arg1, HIGH);
            Serial.println("OK"); 
            break;
        // I/O模式设置 c
        case PIN_MODE:
            if (arg2 == 0) pinMode(arg1, INPUT);
            else if (arg2 == 1) pinMode(arg1, OUTPUT);
            Serial.println("OK");
            break;
        // 编码计数读取 e
        case READ_ENCODERS:
            Serial.print(readEncoder(LEFT));
            Serial.print(" ");
            Serial.println(readEncoder(RIGHT));
            break;
        // 重置编码计数
        case RESET_ENCODERS:
            resetEncoders();
            resetPID();
            Serial.println("OK");
            break;
        // 电机控制 m
        case MOTOR_SPEEDS:
            /* 重置自动停止计时器 */
            lastMotorCommand = millis();
            if (arg1 == 0 && arg2 == 0) {
                setMotorSpeeds(0, 0);
                resetPID();
                moving = 0;
            }
            // 设置小车可移动标志位
            else moving = 1;
            // 目标值设定
            leftPID.TargetTicksPerFrame = arg1;
            rightPID.TargetTicksPerFrame = arg2;
            Serial.println("OK"); 
            break;
        // PID参数设置 u
        case UPDATE_PID:
            /* 
                strtok_s函数是linux下分割字符串的安全函数，函数声明如下：
                char *strtok_r(char *str, const char *delim, char **saveptr);
                该函数也会破坏带分解字符串的完整性，但是其将剩余的字符串保存在saveptr变量中，保证了安全性。
            */
            while ((str = strtok_r(p, ":", &p)) != '\0') {
                // 将分割字符串中的PID参数值依次转换为数字存储在数组中
                pid_args[i] = atoi(str);
                // 索引
                i++;
            }
            Kp = pid_args[0];
            Kd = pid_args[1];
            Ki = pid_args[2];
            Ko = pid_args[3];
            Serial.println("OK");
            break;
        // 错误指令处理
        default:
            Serial.println("Invalid Command");
            break;
    }
}

/**********************************************************
 * 函数名称：serialEvent()
 * 函数功能：串口中断，当串口发送数据来时就会触发此函数，功能命令
 * 存储在cmd中；第一个参数时将参数数据存储在argv1中，后续数据存储
 * 在argv2中。
 * serialEvent()是IDE1.0及以后版本新增的功能，相当于外部中断
 **********************************************************/
void serialEvent() {
    // 读取串口命令
    while (Serial.available() > 0) {
        // 读取串口数据
        chr = Serial.read();
        // 使用 CR 终止命令
        if (chr == 13) {
            if (arg == 1) argv1[index] = NULL;
            else if (arg == 2) argv2[index] = NULL;
            runCommand();       // 执行命令
            resetCommand();     // 重置命令数据
        }
        // 使用空格间隔
        else if (chr == ' ') {
            // 单个参数
            if (arg == 0) arg = 1;
            // 多个参数，对index复位
            else if (arg == 1)  {
                argv1[index] = NULL;
                arg = 2;
                index = 0;
            }
            continue;
        }
        // 读取有效指令
        else {
            // 第一个命令存储在cmd里面
            if (arg == 0) {
                // 第一个 arg 是单字母命令
                cmd = chr;
            }
            // 仅包含一个参数数据接收
            else if (arg == 1) {
                // 后续参数可以是多个字符
                argv1[index] = chr;
                // 索引下标加1
                index++;
            }
            // 包含两个及以上数据指令接收
            else if (arg == 2) {
                argv2[index] = chr;
                index++;
            }
        }
    }
}

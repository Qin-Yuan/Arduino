//公众号：覃原
//实验二：按键控制LED

#define LED 3 //控制LED灯引脚定义为 IO口2
#define button 2 //按键引脚定义为 IO口3

int value = 0 ; //变量value用于存储读取的IO口信息
bool but = false; //设置布尔变量：true/false

//初始化设置以及按键复位
void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT); //LED引脚为输出
  pinMode(button,INPUT);  //button引脚为输入
  digitalWrite(LED,LOW);  //初始化置LED为低电平
  digitalWrite(button,LOW); //初始化置button为低电平
}

//主函数
void loop() {
  // put your main code here, to run repeatedly:
  value = digitalRead(button); //读取按键IO口电平并赋值给变量value
  if(value==HIGH) //检测按键是否按下
  {
    delay(20); //延时20ms,按键消抖
    value = digitalRead(button); //读取按键IO口电平并赋值给变量value
    if(value==HIGH)
    {
      but=!but;  //将布尔变量取反：false->ture 或者 true->false
      if(but==true)  //当but为true时点亮LED
      {
        digitalWrite(LED,HIGH);  //点亮LED灯
      }
      else  //当but为false时熄灭LED灯
      {
        digitalWrite(LED,LOW);  //点亮LED灯
      }
      while(digitalRead(button)==HIGH); //松手检测
    }
  }
}

//公众号：覃原
//实验一：点亮LED灯

int LED = 2;  //控制LED灯引脚定义

void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);  //输出模式
  digitalWrite(LED,LOW);  //置为低电平
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED,HIGH);  //led置为高电平，亮起
  delay(1000);             //延时 1s
  digitalWrite(LED,LOW);   //led置为低电平，熄灭
  delay(1000);             //延时 1s
}

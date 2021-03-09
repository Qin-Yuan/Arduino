//公众号：覃原
//实验三：流水灯

int LED[6]={2,3,4,5,6,7}; //用数组定义IO口

void setup() {
  // put your setup code here, to run once:
  int i=0;
  //for循环进行IO口初始化
  for(i=0;i<6;i++)
  {
    pinMode(LED[i],OUTPUT);
    digitalWrite(LED[i],LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int i=0;
  //for循环使电路LED灯流水点亮
  for(i=0;i<6;i++)
  {
    digitalWrite(LED[i],HIGH);
    delay(300);
    digitalWrite(LED[i],LOW);
  }
}

//公众号：覃原
//实验六：四位数码管

//数组定义数码管 a~h 管脚
//          a b c d e f g h
int pin[8]={2,3,4,5,6,7,8,9};
//控制数码管使能位
//        D1   D2   D3   D4
int D[4]={10 , 11 , 12 , 13};
//################
// 片选控制数码管函数
//################
/*
 * 参数a控制数码管位数：0~4,0表示全部
 * 参数b为状态控制：0表示不显示，1表示显示
*/
void light(int a,int b) 
{
  int i=0;
  //控制所有数码管
  if(a==0)
  {
    //全部熄灭
    if(b==0){
      for(i=0;i<4;i++)
      {
        pinMode(D[i],OUTPUT);
        //片选高电平有效
        digitalWrite(D[i],LOW);  
      }
    }
    //全部亮起
    else if(b==1){
      for(i=0;i<4;i++)
      {
        pinMode(D[i],OUTPUT);
        //片选高电平有效
        digitalWrite(D[i],HIGH);  
      }
    }
  }
  //控制单个数码管---亮起
  else if(a!=0&&b==1&&a<5)
  {
    digitalWrite(D[a-1],HIGH);  
  }
  //控制单个数码管--熄灭
  else if(a!=0&&b==0&&a<5)
  {
    digitalWrite(D[a-1],LOW);
  }
  //输入错误情况
  else
  {
    Serial.println("输入错误");
  }
}
//################
// 数码管显示数字 
//################
/*
 * 参数n:需要显示的数字 0~9
 * 参数m:是否显示小数点 0为不显示；1为显示
 */
void show(int n,int m)
{
  int i=0;
  //不显示小数点
  if(m==0)
  {
    digitalWrite(pin[7],HIGH);
    // 数字 ：0
    if(n==0){
        num(0);
    }
    // 数字 ：1
    else if(n==1){
        num(1);
    }
    // 数字 ：2
    else if(n==2){
        num(2);
    }
    // 数字 ：3
    else if(n==3){
        num(3);
    }
    // 数字 ：4
    else if(n==4){
        num(4);
    }
    // 数字 ：5
    else if(n==5){
        num(5);
    }
    // 数字 ：6
    else if(n==6){
        num(6);
    }
    // 数字 ：7
    else if(n==7){
        num(7);
    }
    // 数字 ：8
    else if(n==8){
       num(8);
    }
    // 数字 ：9
    else if(n==9){
        num(9);
    }
    else {
       Serial.println("输入错误");
    }
  }
  else if(m==1)
  {
    digitalWrite(pin[7],LOW);
    // 数字 ：0
    if(n==0){
        num(0);
    }
    // 数字 ：1
    else if(n==1){
        num(1);
    }
    // 数字 ：2
    else if(n==2){
        num(2);
    }
    // 数字 ：3
    else if(n==3){
        num(3);
    }
    // 数字 ：4
    else if(n==4){
        num(4);
    }
    // 数字 ：5
    else if(n==5){
        num(5);
    }
    // 数字 ：6
    else if(n==6){
        num(6);
    }
    // 数字 ：7
    else if(n==7){
        num(7);
    }
    // 数字 ：8
    else if(n==8){
       num(8);
    }
    // 数字 ：9
    else if(n==9){
        num(9);
    }
    else {
       Serial.println("输入错误");
    }
  }
  else {
       Serial.println("输入错误");
    }  
}
//################
// 数码管数字 
//################
/*
 * 参数n:需要显示的数字
 */
void num(int n)
{
// 数字 ：0
    if(n==0){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],LOW);  // d
        digitalWrite(pin[4],LOW);  // e
        digitalWrite(pin[5],LOW);  // f
        digitalWrite(pin[6],HIGH);  // g
    }
    // 数字 ：1
    else if(n==1){
        digitalWrite(pin[0],HIGH);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],HIGH);  // d
        digitalWrite(pin[4],HIGH);  // e
        digitalWrite(pin[5],HIGH);  // f
        digitalWrite(pin[6],HIGH);  // g
    }
    // 数字 ：2
    else if(n==2){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],HIGH);  // c
        digitalWrite(pin[3],LOW);  // d
        digitalWrite(pin[4],LOW);  // e
        digitalWrite(pin[5],HIGH);  // f
        digitalWrite(pin[6],LOW);  // g
    }
    // 数字 ：3
    else if(n==3){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],LOW);  // d
        digitalWrite(pin[4],HIGH);  // e
        digitalWrite(pin[5],HIGH);  // f
        digitalWrite(pin[6],LOW);  // g
    }
    // 数字 ：4
    else if(n==4){
        digitalWrite(pin[0],HIGH);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],HIGH);  // d
        digitalWrite(pin[4],HIGH);  // e
        digitalWrite(pin[5],LOW);  // f
        digitalWrite(pin[6],LOW);  // g
    }
    // 数字 ：5
    else if(n==5){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],HIGH);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],LOW);  // d
        digitalWrite(pin[4],HIGH);  // e
        digitalWrite(pin[5],LOW);  // f
        digitalWrite(pin[6],LOW);  // g
    }
    // 数字 ：6
    else if(n==6){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],HIGH);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],LOW);  // d
        digitalWrite(pin[4],LOW);  // e
        digitalWrite(pin[5],LOW);  // f
        digitalWrite(pin[6],LOW);  // g
    }
    // 数字 ：7
    else if(n==7){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],HIGH);  // d
        digitalWrite(pin[4],HIGH);  // e
        digitalWrite(pin[5],HIGH);  // f
        digitalWrite(pin[6],HIGH);  // g
    }
    // 数字 ：8
    else if(n==8){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],LOW);  // d
        digitalWrite(pin[4],LOW);  // e
        digitalWrite(pin[5],LOW);  // f
        digitalWrite(pin[6],LOW);  // g
    }
    // 数字 ：9
    else if(n==9){
        digitalWrite(pin[0],LOW);  // a
        digitalWrite(pin[1],LOW);  // b
        digitalWrite(pin[2],LOW);  // c
        digitalWrite(pin[3],HIGH);  // d
        digitalWrite(pin[4],HIGH);  // e
        digitalWrite(pin[5],LOW);  // f
        digitalWrite(pin[6],LOW);  // g
    } 
    else {
       Serial.println("输入错误");
    } 
}
void setup()
{
  //设置串口波特率为9600，用于串口输出数字
  Serial.begin(9600);
  //for循环初始化引脚为输出模式，并置高电平测试是否全亮
  int i=0;
  for(i=0;i<8;i++)
  {
    pinMode(pin[i],OUTPUT);
    //4段共阳数码管,低电平有效
    digitalWrite(pin[i],LOW); 
  }
  //片选引脚初始化
  for(i=0;i<4;i++)
  {
    pinMode(D[i],OUTPUT);
    //片选高电平有效
    digitalWrite(D[i],LOW);  
  }
  //依次测试数码管是否正常亮起
  Serial.println("数码管检测");
  for(i=0;i<4;i++)
  {
    //在串口调试处输出亮灯的片选号
    Serial.println(i);
    digitalWrite(D[i],HIGH); 
    delay(1000);
    digitalWrite(D[i],LOW); 
  }  
}
void loop()
{
   int i; 
  //全部亮起
  light(0,1);
  delay(1000);
  //全部熄灭
  light(0,0);
  delay(1000);
  //显示数字1029，延时显示
  for(i=0;i<1000;i++){
    // 1
    light(0,0);
    show(1,0);
    light(1,1);
    delay(1);
    // 0
    light(0,0);
    show(0,0);
    light(2,1);
    delay(1);
    // 2
    light(0,0);
    show(2,0);
    light(3,1);
    delay(1);
    // 9
    light(0,0);
    show(9,0);
    light(4,1);
    delay(1);
  }
  //10倒计时
  for(i=9;i>=0;i--)
  {
    light(0,0);
    show(i,0);
    light(4,1);
    delay(1000);
    }
}

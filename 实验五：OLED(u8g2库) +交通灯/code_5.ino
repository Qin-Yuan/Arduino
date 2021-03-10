//公众号：覃原
//实验五：OLED(u8g2库) +交通灯

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

//iic驱动方式
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   

int redled =2; //定义数字8 接口
int yellowled =3; //定义数字7 接口
int greenled =4; //定义数字4 接口

uint8_t m;  //用于OLED显示数字
int i;  //用于for循环

void setup() {
  u8g2.begin();  //U8G2构造函数  
  pinMode(redled, OUTPUT);//定义红色小灯接口为输出接口
  pinMode(yellowled, OUTPUT); //定义黄色小灯接口为输出接口
  pinMode(greenled, OUTPUT); //定义绿色小灯接口为输出接口             
}
void loop() {
  /*
  ##############
  #### 绿灯 #####
  */
  digitalWrite(greenled, HIGH);//点亮 绿灯
  m=5;
  for(i=0;i<5;i++)
  {
  char m_str[3];
  strcpy(m_str, u8x8_u8toa(m, 1));  //通过赋值法，将数字转为字符串，取1位
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso62_tn); //字体设置
    u8g2.drawStr(40,63,m_str); //动态显示
  } while ( u8g2.nextPage() ); //刷新页面
  delay(1000);//延时1秒
  m--;
  }
  digitalWrite(greenled, LOW); //熄灭 绿灯
  /*
  ##############
  #### 黄灯 #####
  */
  m=3;
  for(int i=0;i<3;i++)//闪烁交替三次，黄灯闪烁效果
  {
    digitalWrite(yellowled, HIGH);//点亮  黄灯
    char m_str[3];
    strcpy(m_str, u8x8_u8toa(m, 1));    /* convert m to a string with two digits */
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_logisoso62_tn); //字体设置
      u8g2.drawStr(40,63,m_str); //动态显示
    } while ( u8g2.nextPage() ); //刷新页面
      delay(500);//延时0.5 秒 
      digitalWrite(yellowled, LOW);//熄灭  黄灯
      delay(500);//延时0.5 秒
      m--;
  } 
  /*
  ##############
  #### 红灯 #####
  */
  digitalWrite(redled, HIGH);//点亮 红灯
  m=8;
  for(i=0;i<8;i++)
  {
  char m_str[3];
  strcpy(m_str, u8x8_u8toa(m, 1));    /* convert m to a string with two digits */
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso62_tn); //字体设置
    u8g2.drawStr(40,63,m_str); //动态显示
  } while ( u8g2.nextPage() ); //刷新页面
  delay(1000);//延时1秒
  m--;
  }
  digitalWrite(redled, LOW);//熄灭 红灯
}

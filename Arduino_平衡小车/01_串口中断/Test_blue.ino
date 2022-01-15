/****************************************************************************
 * Ardino综合项目: Aruino nano平衡小车__1__串口中断
 * Author: Benny
 * 功能：当串口接收到数据时触发串口中断，并将数据以字符串格式存储在变量中
 * 如果串口数据接收完成标志位为true时输出接收到的数据。
****************************************************************************/

unsigned char r_data[30];  // 接收数据，r_data[0]表示接收的数据个数，[1]后面为数据
bool r_flag = false ;      // 接收数据完成标志位

void setup() {
    Serial.begin(115200);
    Serial.println("串口中断数据接收!");
}

void loop() {
    int i ;
    // 接收完成后，处理发送帧
    if(r_flag == true){
        delay(500);
        Serial.print("接收到数据: ");   // 串口输出接收的数据
        for(i=1;i<=r_data[0];i++){
            Serial.write(r_data[i]);   // 输出数据
        }
        r_flag = false;                // 串口数据接收完成标志位置为false
    }
}

/**********************************************************
 * 函数名称：serialEvent()
 * 函数功能：串口中断，当串口发送数据来时就会触发此函数
 * serialEvent()是IDE1.0及以后版本新增的功能，相当于外部中断
 **********************************************************/
void serialEvent() {
    while(r_flag == false) {
        int i = 1;
        unsigned char data ;             // 记录接收的数据
        static unsigned int t = 0 ;      // 计时数据是否发送完成
        static bool flag = false ;       // 数据开始标志位
        static unsigned int num = 0 ;    // 记录 r_data 下标
        while(Serial.available() > 0){   // 发送数据
            flag = true ;                // 数据接收开始标志位
            data = char(Serial.read());  // 将数据转换为char型
            if(flag == true){
                num++;                   // 记录数据位数
                r_data[num] = data ;     // 将数据存储到接收数据变量里
            }
        }
        // 接收数据完毕后计时，超时没有接收就初始化所有标志位
        if(Serial.available() <= 0 && flag == true) {
            t++ ;                // 自加1
        }
        // 数据接收接收完后将所有标志位清零
        if(t >= 2000) {
            r_data[0] = num ;    // 记录接收的数据个数
            flag = false ;       // 局部数据接收完成标志位
            r_flag = true ;      // 全局数据接收完成标志位
            t = 0 ;              // 清除标志位
            num = 0 ;
        }
    }
}

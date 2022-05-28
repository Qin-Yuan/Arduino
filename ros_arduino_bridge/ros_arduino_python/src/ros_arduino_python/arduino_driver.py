#!/usr/bin/env python

"""
    A Python driver for the Arduino microcontroller running the
    ROSArduinoBridge firmware.

    Created for the Pi Robot Project: http://www.pirobot.org
    Copyright (c) 2012 Patrick Goebel.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details at:

    http://www.gnu.org/licenses/gpl.html

"""

import _thread
from math import pi as PI, degrees, radians
import os
import time
import sys, traceback
from serial.serialutil import SerialException
from serial import Serial

SERVO_MAX = 180
SERVO_MIN = 0

class Arduino:
    ''' Configuration Parameters配置参数
    '''
    N_ANALOG_PORTS = 6
    N_DIGITAL_PORTS = 12

    def __init__(self, port="/dev/ttyUSB0", baudrate=57600, timeout=0.5, motors_reversed=False):

        self.PID_RATE = 30 # Do not change this!  It is a fixed property of the Arduino PID controller.
        self.PID_INTERVAL = 1000 / 30

        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.encoder_count = 0
        self.writeTimeout = timeout
        self.interCharTimeout = timeout / 30.
        self.motors_reversed = motors_reversed
        # Keep things thread safe
        self.mutex = _thread.allocate_lock()

        # An array to cache analog sensor readings
        self.analog_sensor_cache = [None] * self.N_ANALOG_PORTS

        # An array to cache digital sensor readings
        self.digital_sensor_cache = [None] * self.N_DIGITAL_PORTS

    def connect(self):
        try:
            print ("Connecting to Arduino on port", self.port, "...")
            self.port = Serial(port=self.port, baudrate=self.baudrate, timeout=self.timeout, writeTimeout=self.writeTimeout)
            # The next line is necessary to give the firmware time to wake up.
            time.sleep(1)
            test = self.get_baud()
            if test != self.baudrate:
                time.sleep(1)
                test = self.get_baud()
                if test != self.baudrate:
                    raise SerialException
            print ("Connected at", self.baudrate)
            print ("Arduino is ready.")

        except SerialException:
            print ("Serial Exception:")
            print (sys.exc_info())
            print ("Traceback follows:")
            traceback.print_exc(file=sys.stdout)
            print ("Cannot connect to Arduino!")
            os._exit(1)

    def open(self):
        ''' Open the serial port.打开串口。
        '''
        self.port.open()

    def close(self):
        ''' Close the serial port.
        '''
        self.port.close()

    def send(self, cmd):
        ''' This command should not be used on its own: it is called by the execute commands
            below in a thread safe manner.此命令不应单独使用：它由execute命令调用以线程安全的方式。
        '''
        self.port.write(cmd + '\r')

    def recv(self, timeout=0.5):
        timeout = min(timeout, self.timeout)
        ''' This command should not be used on its own: it is called by the execute commands
            below in a thread safe manner.  Note: we use read() instead of readline() since
            readline() tends to return garbage characters from the Arduino此命令不应单独使用：它由execute命令调用
以线程安全的方式。注意：我们使用read（）而不是readline（），因为readline（）倾向于从Arduino返回垃圾字符 
        '''
        c = ''
        value = ''
        attempts = 0
        while c != b'\r':
            c = self.port.read(1)
            value += str(c,encoding = "utf-8")
            attempts += 1
            if attempts * self.interCharTimeout > timeout:
                return None

        value = value.strip('\r')
        # print("收到数据：",value,"\n")
        return value

    def recv_ack(self):
        ''' This command should not be used on its own: it is called by the execute commands
            below in a thread safe manner.此命令不应单独使用：它由下面的execute命令以线程安全的方式调用。
        '''
        ack = self.recv(self.timeout)
        return ack == 'OK'

    def recv_int(self):
        ''' This command should not be used on its own: it is called by the execute commands
            below in a thread safe manner.此命令不应单独使用：它由下面的execute命令以线程安全的方式调用。
        '''
        value = self.recv(self.timeout)
        try:
            return int(value)
        except:
            return None

    def recv_array(self):
        ''' This command should not be used on its own: it is called by the execute commands
            below in a thread safe manner.此命令不应单独使用：它由下面的execute命令以线程安全的方式调用。
        '''
        try:
            values = self.recv(self.timeout * self.N_ANALOG_PORTS).split()
            values[0] = int(values[0])
            values[1] = int(values[1])
            #print("收到数据：",values[0],values[1],values,"\n")
            return values
        except:
            return []

    def execute(self, cmd):
        ''' Thread safe execution of "cmd" on the Arduino returning a single integer value.
        线程安全地在Arduino上执行“cmd”，返回单个整数值。
        '''
        self.mutex.acquire()

        try:
            self.port.flushInput()
        except:
            pass

        ntries = 1
        attempts = 0

        try:
            self.port.write((cmd + '\r').encode())
            value = self.recv(self.timeout)
            while attempts < ntries and (value == '' or value == 'Invalid Command' or value == None):
                try:
                    self.port.flushInput()
                    self.port.write((cmd + '\r').encode())
                    value = self.recv(self.timeout)
                except:
                    print ("Exception executing command: " + cmd)
                attempts += 1
        except:
            self.mutex.release()
            print ("Exception executing command: " + cmd)
            value = None

        self.mutex.release()
        return int(value)

    def execute_array(self, cmd):
        ''' Thread safe execution of "cmd" on the Arduino returning an array.
        线程安全地在Arduino上执行“cmd”，返回数组。
        '''
        self.mutex.acquire()

        try:
            self.port.flushInput()
        except:
            pass

        ntries = 1
        attempts = 0

        try:
            self.port.write((cmd + '\r').encode())
            values = self.recv_array()
            # print("收到数据：",values,"\n")
            while attempts < ntries and (values == '' or values == 'Invalid Command' or values == [] or values == None):
                try:
                    self.port.flushInput()
                    self.port.write((cmd + '\r').encode())
                    values = self.recv_array()
                except:
                    print("Exception executing command: " + cmd)
                attempts += 1
        except:
            self.mutex.release()
            print ("Exception executing command: " + cmd)
            raise SerialException
            return []

        try:
            #values = map(int, values)
            values[0] = int(values[0])
            values[1] = int(values[1])
        except:
            values = []

        self.mutex.release()
        #print("收到数据：",values,"\n")
        return values

    def execute_ack(self, cmd):
        ''' Thread safe execution of "cmd" on the Arduino returning True if response is ACK.
        线程安全地在Arduino上执行“cmd”，如果响应为ACK，则返回True。
        '''
        self.mutex.acquire()

        try:
            self.port.flushInput()
        except:
            pass

        ntries = 1
        attempts = 0

        try:
            self.port.write((cmd + '\r').encode())
            ack = self.recv(self.timeout)
            while attempts < ntries and (ack == '' or ack == 'Invalid Command' or ack == None):
                try:
                    self.port.flushInput()
                    self.port.write((cmd + '\r').encode())
                    ack = self.recv(self.timeout)
                except:
                    print ("Exception executing command: " + cmd)
            attempts += 1
        except:
            self.mutex.release()
            print ("execute_ack exception when executing", cmd)
            print (sys.exc_info())
            return 0

        self.mutex.release()
        return ack == 'OK'

    def update_pid(self, Kp, Kd, Ki, Ko):
        ''' Set the PID parameters on the Arduino
        在Arduino上设置PID参数
        '''
        print ("Updating PID parameters")
        cmd = 'u '+str(Kp) + ':' + str(Kd) + ':' + str(Ki) + ':' + str(Ko)
        
        self.execute_ack(cmd)

    def get_baud(self):
        ''' Get the current baud rate on the serial port.
        获取串行端口上的当前波特率。
        '''
        try:
            return int(self.execute('b'));
        except:
            return None

    def get_encoder_counts(self):
        values = self.execute_array('e')
        # values = list(map(values))
        # print("收到数据：",values,"\n")
        if len(values) != 2:
            print ("Encoder count was not 2")
            raise SerialException
            return None
        else:
            if self.motors_reversed:
                values[0], values[1] = -values[0], -values[1]
            return values

    def reset_encoders(self):
        ''' Reset the encoder counts to 0将编码器计数重置为0
        '''
        return self.execute_ack('r')

    def drive(self, right, left):
        ''' Speeds are given in encoder ticks per PID interval速度以每个PID间隔的编码器刻度表示
        '''
        if self.motors_reversed:
            left, right = -left, -right
        return self.execute_ack('m %d %d' %(right, left))

    def drive_m_per_s(self, right, left):
        ''' Set the motor speeds in meters per second.以米/秒为单位设置电机速度
        '''
        left_revs_per_second = float(left) / (self.wheel_diameter * PI)
        right_revs_per_second = float(right) / (self.wheel_diameter * PI)

        left_ticks_per_loop = int(left_revs_per_second * self.encoder_resolution * self.PID_INTERVAL * self.gear_reduction)
        right_ticks_per_loop  = int(right_revs_per_second * self.encoder_resolution * self.PID_INTERVAL * self.gear_reduction)

        self.drive(right_ticks_per_loop , left_ticks_per_loop )

    def stop(self):
        ''' Stop both motors.停止两个马达。
        '''
        self.drive(0, 0)

    def analog_read(self, pin):
        return self.execute('a %d' %pin)

    def analog_write(self, pin, value):
        return self.execute_ack('x %d %d' %(pin, value))

    def digital_read(self, pin):
        return self.execute('d %d' %pin)

    def digital_write(self, pin, value):
        return self.execute_ack('w %d %d' %(pin, value))

    def pin_mode(self, pin, mode):
        return self.execute_ack('c %d %d' %(pin, mode))

    def servo_write(self, id, pos):
        ''' Usage: servo_write(id, pos)
            Position is given in radians and converted to degrees before sending
        '''
        return self.execute_ack('s %d %d' %(id, min(SERVO_MAX, max(SERVO_MIN, degrees(pos)))))

    def servo_read(self, id):
        ''' Usage: servo_read(id)用法：伺服写入（id、pos）
            The returned position is converted from degrees to radians位置以弧度表示，并在发送前转换为度
        '''
        return radians(self.execute('t %d' %id))

    def ping(self, pin):
        ''' The srf05/Ping command queries an SRF05/Ping sonar sensor
            connected to the General Purpose I/O line pinId for a distance,
            and returns the range in cm.  Sonar distance resolution is integer based.
            srf05/Ping命令查询连接到通用I/O线pinId的srf05/Ping声纳传感器的距离，
            并返回以厘米为单位的范围。声纳距离分辨率是基于整数的。
        '''
        return self.execute('p %d' %pin);

#    def get_maxez1(self, triggerPin, outputPin):
#        ''' The maxez1 command queries a Maxbotix MaxSonar-EZ1 sonar
#            sensor connected to the General Purpose I/O lines, triggerPin, and
#            outputPin, for a distance, and returns it in Centimeters. NOTE: MAKE
#            SURE there's nothing directly in front of the MaxSonar-EZ1 upon
#            power up, otherwise it wont range correctly for object less than 6
#            inches away! The sensor reading defaults to use English units
#            (inches). The sensor distance resolution is integer based. Also, the
#            maxsonar trigger pin is RX, and the echo pin is PW.
#maxez1命令查询Maxbotix MaxSonar-EZ1声纳
#连接到通用I/O线、触发器引脚和
#outputPin，表示距离，并以厘米为单位返回。注：制作
#当然MaxSonar-EZ1正前方没有任何东西
#通电，否则对于小于6的物体，其范围将不正确
#几英寸远！传感器读数默认使用英制单位
#（英寸）。传感器距离分辨率基于整数。还有
#maxsonar触发引脚为RX，回波引脚为PW。
#        '''
#        return self.execute('z %d %d' %(triggerPin, outputPin))


""" Basic test for connectivity 连通性的基本测试"""
if __name__ == "__main__":
    if os.name == "posix":
        portName = "/dev/ttyUSB0"
    else:
        portName = "COM43" # Windows style COM port.

    baudRate = 57600

    myArduino = Arduino(port=portName, baudrate=baudRate, timeout=0.5)
    myArduino.connect()

    print ("Sleeping for 1 second...")
    time.sleep(1)

    print ("Reading on analog port 0", myArduino.analog_read(0))
    print ("Reading on digital port 0", myArduino.digital_read(0))
    print ("Blinking the LED 3 times")
    for i in range(3):
        myArduino.digital_write(13, 1)
        time.sleep(1.0)
    #print "Current encoder counts", myArduino.encoders()

    print ("Connection test successful.",)

    myArduino.stop()
    myArduino.close()

    print ("Shutting down Arduino.")

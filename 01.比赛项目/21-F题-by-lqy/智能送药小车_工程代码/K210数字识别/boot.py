import sensor,image,lcd,time
import KPU as kpu
from machine import UART
from fpioa_manager import fm

from machine import Timer,PWM
import time

#PWM通过定时器配置，接到IO17引脚
tim1 = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PWM)
S1 = PWM(tim1, freq=50, duty=0, pin=17)#水平
tim2 = Timer(Timer.TIMER1, Timer.CHANNEL0, mode=Timer.MODE_PWM)
S2 = PWM(tim2, freq=50, duty=0, pin=18)

lcd.init(freq=15000000)
lcd.rotation(2)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
#sensor.set_hmirror(0)
sensor.run(1)
#加载yolov5模型
task = kpu.load("/sd/yolov2.kmodel")
f=open("anchors.txt","r")
anchor_txt=f.read()

#ROI = [(0,0,224,224),(96,0,224,224)]

L=[]
for i in anchor_txt.split(","):
    L.append(float(i))
anchor=tuple(L)
f.close()
f=open("lable.txt","r")
lable_txt=f.read()
lable = lable_txt.split(",")
f.close()
#设置RX、RT引脚
fm.register(9, fm.fpioa.UART1_TX, force=True)
fm.register(10, fm.fpioa.UART1_RX, force=True)
#设置串口通信
uart_A = UART(UART.UART1, 9600, 8, 1, 0, timeout=1000, read_buf_len=4096)
anchor = (0.1766, 0.1793, 0.4409, 0.3797, 0.6773, 0.5954, 1.0218, 0.9527, 2.158, 1.6841)
sensor.set_windowing((224, 224))
a = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)
#classes=["9","1","4","2","3","8","5","6","7" ]
classes =["8","4","3","5","6","7","2","1" ]#8, 4, 3, 5, 6, 7, 2, 1

'''
说明：舵机控制函数
功能：180度舵机：angle:-90至90 表示相应的角度
     360连续旋转度舵机：angle:-90至90 旋转方向和速度值。
    【duty】占空比值：0-100
'''

def Servo(servo,angle):
    servo.duty((angle+90)/180*10+2.5)

def Set_Servo():
    Servo(S1,-8)
    #Servo(S1,0)
    time.sleep(1)
    Servo(S2,89)
    time.sleep(1)


#全局变量，保存初始化识别的数字
intnum = 0
#初始化识别函数
def begin(intnum):
    TF = 1
    #得分序列，放1-8识别的次数，每一帧识别成哪个，对应的位置加一，1-8哪个先到10即最终识别为哪个
    List_score01 = [0]*8
    while(TF):
         img = sensor.snapshot()
         #code是yolo返回的每个矩形框的参数
         #例图中出现两个目标区域的时候：[{"x":9, "y":99, "w":55, "h":82, "value":0.697979, "classid":8, "index":0, "objnum":2}, {"x":137, "y":105, "w":56, "h":67, "value":0.939132, "classid":4, "index":1, "objnum":2}]
         code = kpu.run_yolo2(task, img)
         #print(code)
         if code:
             for i in code:
                #画目标区域矩形框
                a = img.draw_rectangle(i.rect())
                img.draw_string(0, 0, ("intnum: %d" %(intnum)), color=(255, 0, 0), scale=2)
                a = lcd. display(img)
                list1=list(i.rect())
                #print(classes[i.classid()])
                #识别到的加1
                List_score01[int(classes[i.classid()])-1] += 1
                #print(List_score01)
                if(List_score01[0] >= 10):#识别到数字1并存储为目标病房号
                    intnum = 1
                    print(1)
                    TF = 0
                if(List_score01[1] >= 10):#2
                    intnum = 2
                    print(2)
                    TF = 0
                if(List_score01[2] >= 10):#3
                    intnum = 3
                    print(3)
                    TF = 0
                if(List_score01[3] >= 10):
                    intnum = 4
                    #uart_A.write('D')
                    print(4)
                    TF = 0
                if(List_score01[4] >= 10):
                    intnum = 5
                    #uart_A.write('E')
                    print(5)
                    TF = 0
                if(List_score01[5] >= 10):
                    intnum = 6
                    #uart_A.write('F')
                    print(6)
                    TF = 0
                if(List_score01[6] >= 10):
                    intnum = 7
                    #uart_A.write('G')
                    #print(7)
                    TF = 0
                if(List_score01[7] >= 10):
                    intnum = 8
                    #uart_A.write('H')
                    print(8)
                    TF = 0
         else:
            img.draw_string(0, 0, ("intnum: %d" %(intnum)), color=(255, 0, 0), scale=2)
            a = lcd.display(img)
    return intnum


#道路识别函数
def then(intnum):
    while(1):
        #设置二维矩阵，存放每一个矩形框中不同数字识别的次数，一帧中最多有四个目标区域，但这里设置六个，防止有误识别的造成有很多矩形框程序暴死
        List_score02 = [[0]*8] * 6
        intnum02 = [0]*6
        TF = 1
        while(TF):
            img = sensor.snapshot()
            if intnum != 1 and intnum != 2:
                 code = kpu.run_yolo2(task, img)
                 if code:
                     int_i = -1
                     for i in code:
                         int_i +=1
                         a=img.draw_rectangle(i.rect())
                         img.draw_string(0, 0, ("intnum: %d" %(intnum)), color=(255, 0, 0), scale=2)
                         a = lcd. display(img)
                         list1=list(i.rect())#读取目标位置
                         b=(list1[0]+list1[2])/2
                         List_score02[int_i][int(classes[i.classid()]) -1] += 1
                         print(str(List_score02[int_i]) + str(int_i))
                         for ii in range(8):
                         #int_i是第几个框，ii是第几个框中的每个数字
                            if(List_score02[int_i][ii] == 3 and intnum == ii+1):
                            #当3次识别到目标病房号，通过比较目标坐标和中间值，来判断数字的位置
                                if(b < 75):
                                    uart_A.write("L")
                                    print("L")
                                    TF = 0
                                    List_score02 = [[0]*8] * 6
                                    intnum02 = [0]*6
                                if(b > 75):
                                    uart_A.write("R")
                                    print("R")
                                    TF = 0
                                    List_score02 = [[0]*8] * 6
                                    intnum02 = [0]*6
                 else:
                    img.draw_string(0, 0, ("intnum: %d" %(intnum)), color=(255, 0, 0), scale=2)
                    a = lcd.display(img)
                    TF = 0
                    List_score02 = [[0]*8] * 6
                    intnum02 = [0]*6
            elif intnum == 1:
                uart_A.write("L")
                img.draw_string(0, 0, ("intnum: %d" %(intnum)), color=(255, 0, 0), scale=2)
                a = lcd. display(img)
            elif intnum == 2:
                uart_A.write("R")
                img.draw_string(0, 0, ("intnum: %d" %(intnum)), color=(255, 0, 0), scale=2)
                a = lcd. display(img)
    return 0

#执行程序
#time.sleep(3)
time.sleep(3)
Set_Servo()
uart_A.write("S")
intnum = begin(intnum)
#img.draw_string(90, 110, ("intnum: %d" %(intnum)), color=(255, 0, 0), scale=2)
uart_A.write("E")
time.sleep(3)
#if intnum == 1:
    #uart_A.write("L")
#elif intnum == 2:
    #uart_A.write("R")
then(intnum)
#else:
    #then(intnum)
uart_A.deinit()
del uart_A
a = kpu.deinit(task)

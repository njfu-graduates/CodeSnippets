# Untitled - By: Demoness - 周一 5月 9 2022

import pyb, sensor, image, math, time
from pyb import UART
import json
uart = UART(3,9600)
#GRAYSCALE_THRESHOLD = [(0, 50)]
pyb.LED(1).on()
pyb.LED(2).on()
pyb.LED(3).on()
#led = pyb.LED(3)
usb = pyb.USB_VCP()
#led.on()
#[(3, 35, -9, 65, -10, 41)]
red_threshold = [(5, 48, 10, 63, -10, 41)]#[(0, 36, 6, 127, 8, 75)]#[(0, 32, -85, 49, -14, 45)]#[(0, 56, -103, 93, -117, 71)]#[(16, 71, 15, 102, -23, 97)]

#roi_1 = [(20, 0, 120, 20),         #  北
            #(20, 100, 120, 20),     # 南
            #(0, 0, 20, 120),        #  西
            #(140, 0, 20, 120),      #  东
            #(60,40,40,40)]          #   中
#roi_1 = [(32, 0, 108, 20),         #  北
         #(32, 100, 108, 20),     # 南
         #(12, 0, 20, 20),        #  西
         #(140, 0, 20, 20),      #  东
         #(66,40,40,40)]          #   中
#roi_1_num = [0,2,3]
roi_1 = [(20, 0, 120, 20),         #  北
         #(32, 100, 108, 20),     # 南
         (0, 0, 20, 20),        #  西
         (140, 0, 20, 20),      #  东
         #(66,40,40,40)          #   中
        ]
roi_1_num = [0,1,2]

sensor.reset() # 初始化摄像头
sensor.set_pixformat(sensor.RGB565) # 格式为 RGB565.
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(20) # 跳过10帧，使新设置生效
sensor.set_auto_whitebal(True) # turn this off.
sensor.set_auto_gain(False) # must be turned off for color tracking
clock = time.clock()
#low_threshold = (0, 40                         )

#roi_x = [0,40,100,140]
#ROI = [(26, 30, 20, 20),(46, 30, 20, 20),(76, 30, 20, 20),(106, 30, 20, 20),(126, 30, 20, 20)]
ROI = [(0, 30, 40, 20),(40, 30, 20, 20),(70, 30, 20, 20),(100, 30, 20, 20),(120, 30, 40, 20)]

#buf =[0 for i in range(5)]
buf =[0 for i in range(3)]
DI =[0 for i in range(5)]
uart.init(115200, bits=8, parity=None, stop=1)


while(True):
        m = -1
        is_cross = 0
        is_end = 0
        cnt = 0
        img = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)

        #img.binary([low_threshold],invert = 1)
        for r in roi_1:
            m += 1
            blobs = img.find_blobs(red_threshold, roi=r[0:4],pixels_threshold=100, area_threshold=100, merge=True)
            img.draw_rectangle(r[0:4], color=(255,0,0))
            if blobs:
                most_pixels = 0
                largest_blob = 0
                for i in range(len(blobs)):
                #目标区域找到的颜色块（线段块）可能不止一个，找到最大的一个，作为本区域内的目标直线
                    if blobs[i].pixels() > most_pixels:
                        most_pixels = blobs[i].pixels()
                        #merged_blobs[i][4]是这个颜色块的像素总数，如果此颜色块像素总数大于
                        largest_blob = i
                # Draw a rect around the blob.
                img.draw_rectangle(blobs[largest_blob].rect())
                #将此区域的像素数最大的颜色块画矩形和十字形标记出来
                img.draw_cross(blobs[largest_blob].cx(),
                               blobs[largest_blob].cy())
                buf[m] = 1
            else:
                buf[m] = 0

        k = -1;
        for roi2 in ROI:
            k += 1;
            img.draw_rectangle(roi2, color=(0,255,0))#画出ROI
            #根据红色阈值在目标区域内寻找色块目标
            blobs = img.find_blobs(red_threshold, roi=roi2, pixels_threshold=100, area_threshold=100,merge=True)
            if blobs:
                most_pixels = 0
                largest_blob = 0
                for n in range(len(blobs)):
                #目标区域找到的颜色块（线段块）可能不止一个，找到最大的一个，作为本区域内的目标直线
                   if blobs[n].pixels() > most_pixels:
                       most_pixels = blobs[n].pixels()
                       #merged_blobs[i][4]是这个颜色块的像素总数
                       largest_blob = n
                DI[k] = 1
            else:
                DI[k] = 0

        #统计三个ROI中识别到红线的区域的数量
        for i in roi_1_num:
            if buf[i] == 1:
                cnt +=1
        #如果数量为0，则表明均为识别到红线，车辆到达停止线
        if cnt == 0:
            is_end = 1
        #如果数量超过2，则表明到达T字或十字路口
        elif cnt >= 2 and DI[2] == 1:
            is_cross = 1


        img_data = bytearray([0x55,is_end,is_cross,DI[0],DI[1],DI[2],DI[3],DI[4],0x56])

        uart.write(img_data)
        #uart.write('A')

        print(is_end,is_cross,DI)
        #print(clock.fps())

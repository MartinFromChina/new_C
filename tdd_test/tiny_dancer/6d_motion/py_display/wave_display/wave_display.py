# encoding: utf-8
import pylink 
import sched,time
import struct
import math 

from jlink import JlinkInit,JlinkClose,JlinkRead

successed_time = 1000
sixD_data = []
isOK = True
isRead = False
jlink = JlinkInit()

while isOK == True:
    try:
         isRead =  JlinkRead(jlink,sixD_data)
         if (isRead == True):
             print(sixD_data[0])
             print(sixD_data[1])
             print(sixD_data[2])
             print(sixD_data[3])
             print(sixD_data[4])
             print(sixD_data[5])
             if(successed_time > 0):
                successed_time = successed_time - 1
             else :
                break
    except pylink.errors.JLinkRTTException:
        time.sleep(0.1)


JlinkClose(jlink)






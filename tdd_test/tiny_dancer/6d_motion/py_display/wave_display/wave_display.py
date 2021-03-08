# encoding: utf-8
import pylink 
import sched,time
import struct
import math 

from jlink import JlinkInit,JlinkClose,JlinkRead
from wave  import WaveDispaly

isOK = True
successed_time = 10000

jlink = JlinkInit()

while isOK == True:
    try:
         isOK = WaveDispaly(jlink,JlinkRead)
         if(successed_time > 0):
            successed_time = successed_time - 1
         else :
            break
    except pylink.errors.JLinkRTTException:
        time.sleep(0.1)


JlinkClose(jlink)






# encoding: utf-8
import pylink 
import sched,time
import struct
import math 


jlink = pylink.JLink()
jlink.open()
jlink.set_tif(pylink.enums.JLinkInterfaces.SWD)
print(jlink.connect('nrf52832_XXAA'))
print(jlink.core_id())
print(jlink.device_family())
print(jlink.core_name())
isOK = jlink.target_connected()

successed_time = 1000
ref = 0
ref_backup = 0

while isOK == True:
    try:
        list = jlink.memory_read8(0x200045DC,25) # 40 -15
        ref = list[0]
        if(ref_backup != ref):
          ref_backup = ref
          #print(list)
          FLOAT32 = []
          for i in range(0,4,1):FLOAT32.append(list[4-i])
          #print(FLOAT32)
          acce_x = struct.unpack('<f', struct.pack('4B', *FLOAT32))[0] # B means unsigned byte
          print(acce_x)

          FLOAT321 = []
          for i in range(0,4,1):FLOAT321.append(list[8-i])
          acce_y = struct.unpack('<f', struct.pack('4B', *FLOAT321))[0]
          print(acce_y)

          FLOAT322 = []
          for i in range(0,4,1):FLOAT322.append(list[12-i])
          acce_z = struct.unpack('<f', struct.pack('4B', *FLOAT322))[0]
          print(acce_z)

          print('---------------------------------')
          if(successed_time > 0):
            successed_time = successed_time - 1
          else :
            break
    except pylink.errors.JLinkRTTException:
        time.sleep(0.1)


jlink.close()





# encoding: utf-8
import pylink 
import sched,time

jlink = pylink.JLink()
jlink.open()
jlink.set_tif(pylink.enums.JLinkInterfaces.SWD)
print(jlink.connect('nrf52832_XXAA'))
print(jlink.core_id())
print(jlink.device_family())
print(jlink.core_name())
isOK = jlink.target_connected()

successed_time = 100
ref = 0
ref_backup = 0

while isOK == True:
    try:
        list = jlink.memory_read8(0x200045DC,25) # 40 -15
        ref = list[0]
        if(ref_backup != ref):
          ref_backup = ref
          print(list)
          if(successed_time > 0):
            successed_time = successed_time - 1
          else :
            break
    except pylink.errors.JLinkRTTException:
        time.sleep(0.1)


jlink.close()





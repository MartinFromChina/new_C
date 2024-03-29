# encoding: utf-8
import pylink 
import struct

ref = 0
ref_backup = 0
isJlinkInit = False    

def NanHandle(src):
    if(src != src):
        #print('nan detectived')
        return 0.0
    else:
        return src

def FourCharToDouble(four_Char):
    return struct.unpack('>f', struct.pack('4B', *four_Char))[0]
       
def JlinkInit():
    global ref 
    global ref_backup
    global isJlinkInit
    ref = 0
    ref_backup = 0
    p_jlink = pylink.JLink()
    p_jlink.open()
    p_jlink.set_tif(pylink.enums.JLinkInterfaces.SWD)
    print(p_jlink.connect('nrf52832_XXAA'))
    print(p_jlink.core_id())
    print(p_jlink.device_family())
    print(p_jlink.core_name())
    p_jlink.target_connected()
    isJlinkInit = True
    return p_jlink

def JlinkClose(p_jlink):
    p_jlink.close()

def DoesJlinkInitial():
    global isJlinkInit
    return isJlinkInit

def JlinkReadRaw(p_jlink,float_rawdata):
    global ref 
    global ref_backup
    global isJlinkInit
    try:
        list = p_jlink.memory_read8(0x200045FC,25) 
        ref = list[0]
        if(ref_backup != ref):
          ref_backup = ref

          '''
          FLOAT32 = []
          for i in range(0,4,1):FLOAT32.append(list[4-i])
          acce_x = FourCharToDouble(FLOAT32)
          acce_x = NanHandle(acce_x)
          print(acce_x)
          '''
          
          del float_rawdata[:]
          #print(list)
          for i in range(0,23,1):
            float_rawdata.append(list[i+1])
          return True
        else:
            return False
    except :
                print("read error !!!!!!, close jlink ")
                JlinkClose(p_jlink)
                #time.sleep(0.1)
                isJlinkInit = False
                return False

def JlinkRead(p_jlink,float_rawdata):
    global ref 
    global ref_backup
    global isJlinkInit
    try:
        list = p_jlink.memory_read8(0x200045FC,25) 
        ref = list[0]
        if(ref_backup != ref):
          ref_backup = ref
          FLOAT32 = []
          for i in range(0,4,1):FLOAT32.append(list[4-i])
          #print('%#x'%FLOAT32[0])
          #print('%#x'%FLOAT32[1])
          #print('%#x'%FLOAT32[2])
          #print('%#x'%FLOAT32[3])
          acce_x = FourCharToDouble(FLOAT32)
          acce_x = NanHandle(acce_x)
          #print(acce_x)

          FLOAT321 = []
          for i in range(0,4,1):FLOAT321.append(list[8-i])
          acce_y= FourCharToDouble(FLOAT321)
          acce_y = NanHandle(acce_y)
          #print(acce_y)

          FLOAT322 = []
          for i in range(0,4,1):FLOAT322.append(list[12-i])
          acce_z = FourCharToDouble(FLOAT322)
          acce_z = NanHandle(acce_z)
          #print(acce_z)

          FLOAT323 = []
          for i in range(0,4,1):FLOAT323.append(list[16-i])
          ratate_x = FourCharToDouble(FLOAT323)
          #print(ratate_x)
          ratate_x = NanHandle(ratate_x)
          
          FLOAT324 = []
          for i in range(0,4,1):FLOAT324.append(list[20-i])
          ratate_y = FourCharToDouble(FLOAT324)
          #print(ratate_y)
          ratate_y = NanHandle(ratate_y)

          FLOAT325 = []
          for i in range(0,4,1):FLOAT325.append(list[24-i])
          ratate_z = FourCharToDouble(FLOAT325)
          #print(ratate_z)
          ratate_z = NanHandle(ratate_z)

          #print('---------------------------------')
          del float_rawdata[:]
          float_rawdata.append(acce_x)
          float_rawdata.append(acce_y)
          float_rawdata.append(acce_z)
          float_rawdata.append(ratate_x)
          float_rawdata.append(ratate_y)
          float_rawdata.append(ratate_z)
          
          return True
        else:
            return False
    except :
                print("read error !!!!!!, close jlink ")
                JlinkClose(p_jlink)
                #time.sleep(0.1)
                isJlinkInit = False
                return False


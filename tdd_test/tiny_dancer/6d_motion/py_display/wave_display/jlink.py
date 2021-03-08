# encoding: utf-8
import pylink 
import struct

ref = 0
ref_backup = 0
    

def JlinkInit():
    global ref 
    global ref_backup
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
    return p_jlink

def JlinkClose(p_jlink):
    p_jlink.close()



def JlinkRead(p_jlink,float_rawdata):
    global ref 
    global ref_backup
    list = p_jlink.memory_read8(0x200045DC,25) 
    ref = list[0]
    if(ref_backup != ref):
      ref_backup = ref
      FLOAT32 = []
      for i in range(0,4,1):FLOAT32.append(list[4-i])
      #print(FLOAT32)
      acce_x = struct.unpack('<f', struct.pack('4B', *FLOAT32))[0] # B means unsigned byte
      #print(acce_x)

      FLOAT321 = []
      for i in range(0,4,1):FLOAT321.append(list[8-i])
      acce_y= struct.unpack('<f', struct.pack('4B', *FLOAT321))[0]
      #print(acce_y)

      FLOAT322 = []
      for i in range(0,4,1):FLOAT322.append(list[12-i])
      acce_z = struct.unpack('<f', struct.pack('4B', *FLOAT322))[0]
      #print(acce_z)

      FLOAT323 = []
      for i in range(0,4,1):FLOAT323.append(list[16-i])
      ratate_x = struct.unpack('<f', struct.pack('4B', *FLOAT323))[0]

      FLOAT324 = []
      for i in range(0,4,1):FLOAT324.append(list[20-i])
      ratate_y = struct.unpack('<f', struct.pack('4B', *FLOAT324))[0]

      FLOAT325 = []
      for i in range(0,4,1):FLOAT325.append(list[24-i])
      ratate_z = struct.unpack('<f', struct.pack('4B', *FLOAT325))[0]

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




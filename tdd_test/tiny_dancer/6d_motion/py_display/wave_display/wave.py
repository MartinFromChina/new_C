# encoding: utf-8

import matplotlib.pyplot as plt
import matplotlib.font_manager as font_manager
from matplotlib import animation
import numpy as np
from jlink import JlinkInit,DoesJlinkInitial,JlinkClose
from clock import ClockSet
import time

accex_index = 0  
real_p_link = None
x_backup = 1.0

def GetXiTa(x,y,z):
    global x_backup
    '''
    if(x == 0.0):
        print('--------DETECTED---!!!!!!!!!!!!!!!!!!!!!!')
        x = x_backup/2
    '''
    cos_xita = (x*x) + (y*y) + (z*z)
    cos_xita = cos_xita **0.5
    
    cos_xita = x/cos_xita
    cos_xita = -cos_xita 
    x_backup = x
    #print(cos_xita)
    #print('---------------------',x,y,z,'---------------------',cos_xita)
    xita = np.arccos(cos_xita)

    #print('---------------------',xita,xita/3.1415926)
    xita = 180*xita/3.1415926

    #xita = int(xita)
    #print(xita)
    return xita

def ConvertAxis(src,dest,index):
    if(src >= 0):
        dest[index] = src* (10**38)
    else:
        dest[index] = src * (10**(-38))

def ConvertRotate(src,dest,index):
    if(src >= 0):
        dest[index] = src* (10**41)
    else:
        dest[index] = src * (10**(-41))
        
def BufClear(buf,size,value):
    for i in range(0,size,1):
        buf.append(value)

def WaveDispaly(jlink_read):
    sixD_data = [] 
    ACCE_X = []
    ACCE_Y = []
    ACCE_Z = []
    ROTATE_X = []
    ROTATE_Y = []
    ROTATE_Z = []
    global real_p_link
#--------------------------------------------------------
    fig = plt.figure(figsize=(12, 8))
    ax1 = fig.add_subplot(2,1,1)
    ax2 = fig.add_subplot(2,1,2)
#--------------------------------------------------------    
    x1 = np.arange(0,1000,1) 
    acce_x = x1 
    acce_y = -x1
    acce_z = x1 - x1
    line1_x,= ax1.plot(x1,acce_x,color='red',linewidth = 0.5)
    line1_y,= ax1.plot(x1,acce_y,color='green',linewidth = 0.5)
    line1_z,= ax1.plot(x1,acce_z,color='blue',linewidth = 0.5)
    
 #--------------------------------------------------------
    x2 = np.arange(0,1000,1)  
    rota_x = x2 
    rota_y = -x2
    rota_z = x2 - x2

    line2_x,= ax2.plot(x2,rota_x,color='red',linewidth = 0.5)
    line2_y,= ax2.plot(x2,rota_y,color='green',linewidth = 0.5)
    line2_z,= ax2.plot(x2,rota_z,color='blue',linewidth = 0.5)

    #--------------------------------------------------------

     
    def animate(i):
        global accex_index
        global real_p_link
        isInit = DoesJlinkInitial()
        if(isInit == False):
            real_p_link = JlinkInit()
            print("reconnectjlink")
        isNew = jlink_read(real_p_link,sixD_data)
        
        if(isNew == True):
            
            ConvertAxis(sixD_data[0],ACCE_X,accex_index)
            ConvertAxis(sixD_data[1],ACCE_Y,accex_index)
            ConvertAxis(sixD_data[2],ACCE_Z,accex_index)
            ConvertRotate(sixD_data[3],ROTATE_X,accex_index)
            ConvertRotate(sixD_data[4],ROTATE_Y,accex_index)
            ConvertRotate(sixD_data[5],ROTATE_Z,accex_index)

            angle = GetXiTa(ACCE_X[accex_index],ACCE_Y[accex_index],ACCE_Z[accex_index])
           
            
            if (accex_index < 999):
                accex_index = accex_index + 1
            else:
                accex_index = 0
            #print('----------------------------------------------------------')
            ClockSet(angle)
        
        line1_x.set_ydata(np.array(ACCE_X)[(x1 + accex_index )%1000]+500)   
        line1_y.set_ydata(np.array(ACCE_Y)[(x1 + accex_index )%1000])  
        line1_z.set_ydata(np.array(ACCE_Z)[(x1 + accex_index )%1000]-500)  
        
        line2_x.set_ydata(np.array(ROTATE_X)[(x2 + accex_index )%1000]+500)   
        line2_y.set_ydata(np.array(ROTATE_Y)[(x2 + accex_index )%1000])  
        line2_z.set_ydata(np.array(ROTATE_Z)[(x2 + accex_index )%1000]-500) 

        return line1_x,line1_y,line1_z,line2_x,line2_y,line2_z
    #--------------------------------------------------------
    def init():
        BufClear(ACCE_X,1000,500)
        BufClear(ACCE_Y,1000,0)
        BufClear(ACCE_Z,1000,500)
        BufClear(ROTATE_X,1000,500)
        BufClear(ROTATE_Y,1000,0)
        BufClear(ROTATE_Z,1000,500)
        
        line1_x.set_ydata(0)
        line1_y.set_ydata(0)
        line1_z.set_ydata(0)
        ax1.set_ylim(-1000,1000)
        line2_x.set_ydata(0)
        line2_y.set_ydata(0)
        line2_z.set_ydata(0)
        ax2.set_ylim(-1000,1000)
        return line1_x,line1_y,line1_z,line2_x,line2_y,line2_z
   #--------------------------------------------------------     
    ani = animation.FuncAnimation(fig = fig,func=animate,frames = 50,init_func=init,interval = 8,blit = True)
    
    plt.show()
    JlinkClose(real_p_link)

def WaveDispalySimple(p_link,jlink_read):
        sixD_data = [] 
        while True:
            isNew = jlink_read(p_link,sixD_data)
            time.sleep(0.1)
            
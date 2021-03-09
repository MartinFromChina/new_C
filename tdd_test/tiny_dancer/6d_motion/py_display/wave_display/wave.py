# encoding: utf-8

import matplotlib.pyplot as plt
import matplotlib.font_manager as font_manager
from matplotlib import animation
import numpy as np
from tkinter import *
from jlink import JlinkInit,DoesJlinkInitial
import time

accex_index = 0  
real_p_link = 0
def BufClear(buf,size):
    for i in range(0,size,1):
        buf.append(0)

def WaveDispaly(jlink_read):
    sixD_data = [] 
    ACCE_X = []
   
#--------------------------------------------------------
    fig = plt.figure(figsize=(12, 12))
    ax1 = fig.add_subplot(3,1,1)
    ax2 = fig.add_subplot(3,1,2)
#--------------------------------------------------------    
    x1 = np.arange(0,1000,1) 
    acce_x = x1 
    acce_y = -x1
    acce_z = x1 - x1
    line1_x,= ax1.plot(x1,acce_x,color='red')
    line1_y,= ax1.plot(x1,acce_y,color='green')
    line1_z,= ax1.plot(x1,acce_z,color='blue')
 #--------------------------------------------------------
    x2 = np.arange(0,500,1)  
    rota_x = x2 
    rota_y = -x2
    rota_z = x2 - x2

    line2_x,= ax2.plot(x2,rota_x,color='red')
    line2_y,= ax2.plot(x2,rota_y,color='green')
    line2_z,= ax2.plot(x2,rota_z,color='blue')
    
    plt.grid()
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
            #print(sixD_data[0]);print(sixD_data[1]);print(sixD_data[2])
            #print(sixD_data[3]);print(sixD_data[4]);print(sixD_data[5])

            if(sixD_data[0] < 10000):
                ACCE_X[accex_index] = 5000
            else:
                ACCE_X[accex_index] = 15000
            #ACCE_X[accex_index] = sixD_data[0]/1000000
            
            if (accex_index < 999):
                accex_index = accex_index + 1
            else:
                accex_index = 0
            #print(accex_index)
            #print(ACCE_X)
            #print(np.array(ACCE_X))
            #print('----------------------------------------------------------')
            
            
            for i in range(0,1000,1):
                #print((i + accex_index )%1000)
                #print(ACCE_X[i])
                #print(np.array(ACCE_X)[ (i + accex_index + 1)/1000])
                a = 0
            print('----------------------------------------------------------%f'%sixD_data[0])
             
            
            line1_y.set_ydata(acce_y + (i * 200))

        
        line1_x.set_ydata(np.array(ACCE_X)[(x1 + accex_index )%1000])    
        return line1_x,line1_y,line1_z,line2_x,line2_y,line2_z
    #--------------------------------------------------------
    def init():
        BufClear(ACCE_X,1000)
        line1_x.set_ydata(0)
        line1_y.set_ydata(0)
        line1_z.set_ydata(0)
        ax1.set_ylim(-20000,20000)
        line2_x.set_ydata(0)
        line2_y.set_ydata(0)
        line2_z.set_ydata(0)
        return line1_x,line1_y,line1_z,line2_x,line2_y,line2_z
   #--------------------------------------------------------     
    ani = animation.FuncAnimation(fig = fig,func=animate,frames = 50,init_func=init,interval = 8,blit = True)
    
    plt.show()


def WaveDispalySimple(p_link,jlink_read):
        sixD_data = [] 
        while True:
            isNew = jlink_read(p_link,sixD_data)
            time.sleep(0.1)
            
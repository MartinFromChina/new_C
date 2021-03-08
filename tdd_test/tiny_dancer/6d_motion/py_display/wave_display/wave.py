# encoding: utf-8

import matplotlib.pyplot as plt
import matplotlib.font_manager as font_manager
from matplotlib import animation
import numpy as np
  

def WaveDispaly(p_link,jlink_read):
    sixD_data = []
    isNew = jlink_read(p_link,sixD_data)
    if(isNew == True):
         print(sixD_data[0])
         print(sixD_data[1])
         print(sixD_data[2])
         print(sixD_data[3])
         print(sixD_data[4])
         print(sixD_data[5])

    fig = plt.figure(figsize=(12, 12))
    ax1 = fig.add_subplot(3,1,1)
    ax2 = fig.add_subplot(3,1,2)
    
    x1 = np.arange(0,1000,1) 
    acce_x = x1 
    acce_y = -x1
    acce_z = x1 - x1

    x2 = np.arange(0,500,1)  
    y2 = -x2

    y_limit_up_backup = 1000
    y_limit_down_backup = -1000

    line1_x,= ax1.plot(x1,acce_x,color='red')
    line1_y,= ax1.plot(x1,acce_y,color='green')
    line1_z,= ax1.plot(x1,acce_z,color='blue')
    line2,= ax2.plot(x2,y2)
    
    plt.grid()

    def animate(i):
       line1_x.set_ydata(acce_x + (i * 200))
       line1_y.set_ydata(acce_y + (i * 200))
       line2.set_ydata(y2 + (i * 200))
       return line1_x,line1_y,line1_z,line2,

    def init():
        line1_x.set_ydata(0)
        line1_y.set_ydata(0)
        line1_z.set_ydata(0)
        ax1.set_ylim(-2000,2000)
        line2.set_ydata(0)
        return line1_x,line1_y,line1_z,line2, 
        
    ani = animation.FuncAnimation(fig = fig,func=animate,frames = 100,init_func=init,interval = 500,blit = True)
    
    plt.show()

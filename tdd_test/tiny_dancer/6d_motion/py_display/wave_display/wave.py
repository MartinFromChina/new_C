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
    y1 = x1 

    x2 = np.arange(0,500,1)  
    y2 = -x2

    line1,= ax1.plot(x1,y1)
    line2,= ax2.plot(x2,y2)
    
    plt.grid()

    def animate(i):
       line1.set_ydata(y1 + (i * 200))
       line2.set_ydata(y2 + (i * 200))
       return line1,line2,

    def init():
        line1.set_ydata(0)
        line2.set_ydata(0)
        return line1,line2, 
        
    ani = animation.FuncAnimation(fig = fig,func=animate,frames = 100,init_func=init,interval = 500,blit = True)
    
    plt.show()

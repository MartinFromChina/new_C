
# encoding: utf-8

import matplotlib.pyplot as plt 
import numpy as np 
from scipy import array 
from mpl_toolkits.mplot3d import Axes3D
from jlink_read import JlinkClose,DoesJlinkInitial,JlinkRead,JlinkInit


z = 0
accex_index = 0  
real_p_link = None
sixD_data = []


jet = plt.get_cmap('jet') 
from matplotlib import animation 
fig = plt.figure() 
ax = fig.gca(projection='3d') 

X = np.linspace(0,20,5) 
Z = np.linspace(0,20,5) 
X,Z= np.meshgrid(X, Z) 

Y = Z - Z + X - X + 2
pai = 3.141592653


def rotate(x_xi,y_xi,z_xi):
    global pai
    ax.view_init(x_xi,z_xi) 
    ax.clear()

    
    k = pai*y_xi/180
    real_k = np.tan(k)
    Y = real_k*(Z - 10) + 12
    
    ax.set_xlim3d(0, 20) 
    ax.set_ylim3d(0, 20)
    ax.set_zlim3d(0, 20)

    ax.plot_surface(X, Y, Z, rstride = 1, cstride = 1, cmap = jet,linewidth = 0,alpha= 1) 

    
def action(angle): 
    global z
    global accex_index
    global real_p_link
    isInit = DoesJlinkInitial()
    if(isInit == False):
        real_p_link = JlinkInit()
        print("reconnectjlink")
    isNew = JlinkRead(real_p_link,sixD_data)
    
    if(isNew == True):
        a = 0
        x_xita = 0
        y_xita = 0
        z_xita = 45
        rotate(x_xita,y_xita,z_xita)

   
    

    
surf = ax.plot_surface(X, Y, Z, rstride = 1, cstride = 1, cmap = jet,linewidth = 0,alpha= 1) 
#ax.set_xlim3d(0, X.max()) 
#ax.set_zlim3d(0, Z.max())
ax.set_xlim3d(0, 20) 
ax.set_ylim3d(0, 20)
ax.set_zlim3d(0, 20)

fig.colorbar(surf, shrink=0.8, aspect=5) 
ax.set_xlabel('x') 
ax.set_ylabel('y') 
ax.set_zlabel('z') 

rot_animation = animation.FuncAnimation(fig, action, frames=np.arange(0,362,2),interval=8) 

plt.show() 




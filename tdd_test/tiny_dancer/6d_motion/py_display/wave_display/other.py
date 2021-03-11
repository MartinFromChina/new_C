
# encoding: utf-8

import matplotlib.pyplot as plt 
import numpy as np 
from scipy import array 
from mpl_toolkits.mplot3d import Axes3D

surf = None

jet = plt.get_cmap('jet') 
from matplotlib import animation 
fig = plt.figure() 
ax = fig.gca(projection='3d') 

X = np.linspace(0,20,20) 
Y = np.linspace(0,20,20) 
X,Y= np.meshgrid(X, Y) 


'''
Z = array ([ 
     [1223.539555, 1428.075086,1714.479425, 2144.053223], 
     [1567.26647,1829.056119,2990.416079,2745.320067], 
     [2135.163957,2491.534201, 2990.416079,3738.761638], 
     [3257.280827, 3800.655101, 4561.372117, 5702.458776], 
     ]) 
'''
Z = X*X + Y*Y
def rotate(angle): 
    #print(angle)
    ax.view_init(angle,angle) 
    global surf
    ax.clear()
    #Z[0] = Z[0] + angle
    if(((angle /2) % 2) == 0):
       Z = X
       #print(1)
    else:
       Z = 4*X + 3*Y
       #print(0)
    
    surf = ax.plot_surface(X, Y, Z, rstride = 1, cstride = 1, cmap = jet,linewidth = 0,alpha= 1) 
    

surf = ax.plot_surface(X, Y, Z, rstride = 1, cstride = 1, cmap = jet,linewidth = 0,alpha= 1) 
ax.set_zlim3d(0, Z.max()) 

fig.colorbar(surf, shrink=0.8, aspect=5) 
ax.set_xlabel('Axial Length [mm]') 
ax.set_ylabel('nbTurns') 
ax.set_zlabel('RPM') 

rot_animation = animation.FuncAnimation(fig, rotate, frames=np.arange(0,362,2),interval=500) 


plt.show() 


'''


from mpl_toolkits.mplot3d import Axes3D

import matplotlib.pyplot as plt

import numpy as np

n_radii = 8

n_angles = 36

# Make radii and angles spaces (radius r=0 omitted to eliminate duplication).

radii = np.linspace(0.125, 1.0, n_radii)

angles = np.linspace(0, 2*np.pi, n_angles, endpoint=False)

# Repeat all angles for each radius.

angles = np.repeat(angles[..., np.newaxis], n_radii, axis=1)

# Convert polar (radii, angles) coords to cartesian (x, y) coords.

# (0, 0) is manually added at this stage, so there will be no duplicate

# points in the (x, y) plane.

x = np.append(0, (radii*np.cos(angles)).flatten())

y = np.append(0, (radii*np.sin(angles)).flatten())

# Compute z to make the pringle surface.

z = np.sin(-x*y)

fig = plt.figure()

ax = fig.gca(projection='3d')

ax.plot_trisurf(x, y, z, linewidth=0.2, antialiased=True)

steps = 100

theta = np.linspace(0, 2 * np.pi, steps)

r_max = 1.2

x = np.zeros_like(theta)

y = r_max * np.cos(theta)

z = r_max * np.sin(theta)

ax.plot(x, y, z, 'r')

ax.plot(y, x, z, 'g')

ax.plot(z, y, x, 'b')

scale = 1.08

ax.quiver((0,), (0), (0),

(0), (0), (r_max), color=('c'))

ax.text(0, 0, r_max * scale, 'Z Theta', weight='bold')

ax.quiver((0), (0), (0),

(0), (r_max), (0), color=('m'))

ax.text(0, r_max * scale, 0, 'Y', weight='bold')

ax.quiver((0), (0), (0),

(r_max), (0), (0), color=('y'))

ax.text(r_max * scale, 0, 0, 'X', weight='bold')

plt.show()
'''

'''
# -*- coding: utf-8 -*-
# author:           inspurer(月小水长)
# pc_type           lenovo
# create_date:      2019/1/23
# file_name:        3DTest
# github            https://github.com/inspurer
# qq_mail           2391527690@qq.com

# -*- coding: utf-8 -*-
"""
绘制3d图形
"""

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
# 定义figure
fig = plt.figure()
# 创建3d图形的两种方式
# 将figure变为3d
ax = Axes3D(fig)

#ax = fig.add_subplot(111, projection='3d')

# 定义x, y
x = np.arange(-4, 4, 0.25)
y = np.arange(-4, 4, 0.25)

# 生成网格数据
X, Y = np.meshgrid(x, y)

# 计算每个点对的长度
R = np.sqrt(X ** 2 + Y ** 2)
# 计算Z轴的高度
Z = np.sin(R)

# 绘制3D曲面


# rstride:行之间的跨度  cstride:列之间的跨度
# rcount:设置间隔个数，默认50个，ccount:列的间隔个数  不能与上面两个参数同时出现


# cmap是颜色映射表
# from matplotlib import cm
# ax.plot_surface(X, Y, Z, rstride = 1, cstride = 1, cmap = cm.coolwarm)
# cmap = "rainbow" 亦可
# 我的理解的 改变cmap参数可以控制三维曲面的颜色组合, 一般我们见到的三维曲面就是 rainbow 的
# 你也可以修改 rainbow 为 coolwarm, 验证我的结论
ax.plot_surface(X, Y, Z, rstride = 1, cstride = 1, cmap = plt.get_cmap('rainbow'))

# 绘制从3D曲面到底部的投影,zdir 可选 'z'|'x'|'y'| 分别表示投影到z,x,y平面
# zdir = 'z', offset = -2 表示投影到z = -2上
ax.contour(X, Y, Z, zdir = 'z', offset = -2, cmap = plt.get_cmap('rainbow'))

# 设置z轴的维度，x,y类似
ax.set_zlim(-2, 2)

plt.show()
'''

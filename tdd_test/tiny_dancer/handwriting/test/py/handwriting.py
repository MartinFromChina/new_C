# encoding: utf-8
from PIL import Image

# 转化为黑白图
def WhiteWrite(img):
    pixdata = img.load()
    print("sixe x %d : size y %d"%(img.size[0],img.size[1]) )
    for y in xrange(img.size[1]):
        for x in xrange(img.size[0]):
           pixdata[x, y] = (255, 255, 255, 255)        # r g b 透明度         
    img.save('white.png')

    

im = Image.open('src.jpg')
im = im.convert("RGBA")
WhiteWrite(im)
im.close()



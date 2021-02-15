# encoding: utf-8
from PIL import Image


def line():
    im = Image.open('src.jpg')

    out = im.resize((128, 128))
    out = im.rotate(45)
    out.save('line.jpg')
    im.close()

line()

# 转化为黑白图
def blackWrite(img):
    pixdata = img.load()

    for y in xrange(img.size[1]):
        for x in xrange(img.size[0]):
           pixdata[x, y] = (255, 255, 255, 0)                 
    img.save('black.png')

    

im = Image.open('src.jpg')
im = im.convert("RGBA")
blackWrite(im)
im.close()



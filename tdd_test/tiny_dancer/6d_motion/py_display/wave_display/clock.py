# encoding: utf-8

from turtle import *
from datetime import *

def Skip(step):               #建立表的外框
    penup()
    forward(step)
    pendown()

def mkHand(name,length):      #注册turtle形状，建立表针turtle
    reset()
    Skip(-length*0.1)
    begin_poly()
    forward(length*1.1)
    end_poly()
    handForm = get_poly()
    register_shape(name,handForm)

def Init():
    setup( width = 400, height = 400, startx = None, starty = None) 
    global secHand,minHand,hurHand,printer
    mode("logo")             #重置turtle指向北

    mkHand("secHand",125)    #建立三个表针并初始化
    mkHand("minHand",130)
    mkHand("hurHand",90)

    secHand = Turtle()
    secHand.shape("secHand")
    minHand = Turtle()
    minHand.shape("minHand")
    hurHand = Turtle()
    hurHand.shape("hurHand")

    for hand in secHand,minHand,hurHand:
        hand.shapesize(1,1,3)
        hand.speed(0)

    printer = Turtle()        #建立输出文字turtle
    printer.hideturtle()
    printer.penup()

def SetupClock(radius):      #建立表外框
    reset()
    pensize(7)
    for i in range(60):
        Skip(radius)
        if i % 5 == 0:
            forward(20)
            Skip(-radius-20)
        else:
            dot(5)
            Skip(-radius)
        right(6)

def ClockInit():
    tracer(False)
    Init()
    SetupClock(160)
    tracer(True)

def ClockSet(angle):
    secHand.setheading(angle)
    minHand.setheading(90)
    hurHand.setheading(180)

def ClockClose():
    a = 0
    #turtle.bye() # not work
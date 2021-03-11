# encoding: utf-8

from ctypes import *

#load the shared object file
adder = CDLL('../../c/finally.so')

for i in range(100):
    res_int = adder.TestFunction()
    print(res_int)




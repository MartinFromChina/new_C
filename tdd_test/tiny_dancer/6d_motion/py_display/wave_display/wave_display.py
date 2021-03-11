# encoding: utf-8
import sched,time
import math 

from jlink import JlinkInit,JlinkClose,JlinkRead
from wave  import WaveDispaly,WaveDispalySimple
from clock import ClockInit,ClockClose

ClockInit()
WaveDispaly(JlinkRead)
ClockClose()







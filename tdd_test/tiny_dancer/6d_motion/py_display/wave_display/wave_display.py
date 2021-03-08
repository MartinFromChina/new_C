# encoding: utf-8
import sched,time
import math 

from jlink import JlinkInit,JlinkClose,JlinkRead
from wave  import WaveDispaly

jlink = JlinkInit()
WaveDispaly(jlink,JlinkRead)
JlinkClose(jlink)






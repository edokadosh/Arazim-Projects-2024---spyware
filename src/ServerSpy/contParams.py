from ctypes import *
import struct
from enum import IntEnum
from structParams import *

MAX_ADAPTER_NAMELEN = 16


class Params(Union):
    _fields_ = [("sniffP", SniffParams), ("kligP", kligPrams), ("BuggP", BuggPrams)]


# types enum values
KligerType = 1
SnifferType = 2
BuggType = 3


class ContParams(Structure):

    _pack_ = 1

    _fields_ = [("type", c_uint32), ("parameters", Params)]

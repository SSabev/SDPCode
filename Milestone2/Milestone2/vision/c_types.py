from ctypes import *


class InitSignal(Structure):
    _fields_ = [("pitchWidth", c_int), ("pitchHeight", c_int)]


class RegSignal(Structure):
    fields_ = [("x", c_int), ("y", c_int)]


class FrameData(Structure):
    _fields_ = [("yellow_x", c_int), ("yellow_y", c_int), ("yellow_angle", c_float), \
            ("blue_x", c_int), ("blue_y", c_int), ("blue_angle", c_float),
            ("ball_x", c_int), ("ball_y", c_int), ("timestamp", c_uint)]

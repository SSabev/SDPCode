# File: hello1.py

import os

from Tkinter import *
from xml.dom import minidom


def writeVal(val):
    # that, gentlement, is one ugly mother-ef-er!
    newVals = ''
    for i in range(5):
        newVals += str(spinboxes[i].get()) + ' '
    with open(os.getcwd() + "/calibration/pitch0Distortion.xml", 'w') as f:
        f.write('<?xml version="1.0"?>\n\
<opencv_storage>\n\
<defaultDistortion type_id="opencv-matrix">\n\
  <rows>5</rows>\n\
  <cols>1</cols>\n\
  <dt>f</dt>\n\
  <data>')
        f.write(newVals)
        f.write('</data>\n\
</defaultDistortion>\n\
</opencv_storage>')


###### XML #######
f = open(os.getcwd() + "/calibration/pitch0Distortion.xml", 'r')
dom = minidom.parse(f)
data = dom.getElementsByTagName('data')[0].firstChild.nodeValue
vals = data.split(' ')
# print vals
# print(str(vals[2]))


###### GUI ######
root = Tk()

w = Label(root, text="Tweak that bi*ch!")
w.pack()

spinboxes = []

for i in range(5):
    if (i == 1):
        spinboxes.append(Scale(root, from_=-250, to=250, resolution=0.00000000001, \
            orient=HORIZONTAL, length=1500, command=writeVal))
    elif (i == 4):
        spinboxes.append(Scale(root, from_=-2500, to=2500, resolution=0.00000000001, \
            orient=HORIZONTAL, length=1500, command=writeVal))
    else:
        spinboxes.append(Scale(root, from_=-15, to=15, resolution=0.00000000001, \
            orient=HORIZONTAL, length=1500, command=writeVal))
    spinboxes[i].set(float(vals[i]))
    spinboxes[i].pack()

root.mainloop()

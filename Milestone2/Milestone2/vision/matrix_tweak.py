# File: hello1.py

import os

from Tkinter import *
from xml.dom import minidom


def writeVal(val):
    # that, gentlement, is one ugly mother-ef-er!
    newVals = ''
    for i in range(4):
        newVals += str(spinboxes[i].get()) + ' '
    with open(os.getcwd() + "/calibration/pitch0Distortion.xml", 'w') as f:
        f.write('<?xml version="1.0"?>\n\
<opencv_storage>\n\
<defaultDistortion type_id="opencv-matrix">\n\
  <rows>4</rows>\n\
  <cols>1</cols>\n\
  <dt>f</dt>\n\
  <data>\n')
        f.write(newVals)
        f.write('\n</data>\n\
</defaultDistortion>\n\
</opencv_storage>')


###### XML #######
f = open(os.getcwd() + "/calibration/pitch0Distortion.xml", 'r')
dom = minidom.parse(f)
data = dom.getElementsByTagName('data')[0].firstChild.nodeValue
vals = data.split(' ')
print(float(vals[2]))


###### GUI ######
root = Tk()

w = Label(root, text="Tweak that bi*ch!")
w.pack()

spinboxes = []

for i in range(4):
    spinboxes.append(Scale(root, from_=-3, to=3, resolution=0.00000000001, \
        orient=HORIZONTAL, length=800, command=writeVal))
    spinboxes[i].set(float(vals[i]))
    spinboxes[i].pack()

root.mainloop()

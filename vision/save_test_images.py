#Call this script by: python save_test_image pitchnum num_to_capture
from SimpleCV import *
import time
from time import gmtime, strftime
import sys
import os

INPUT_DIR = './input_images'
SLEEP_TIME = 1
NUM = 10 # number of images to capture
pitchnum = 0

if len(sys.argv) > 1:
    pitchnum = int (sys.argv[1])
    
if len(sys.argv) > 2:
    NUM = int (sys.argv[2])


#Find the highest number in existing files
count = 0
for filename in os.listdir(INPUT_DIR):
    x = int(filename[3:-4])
    if x > count:
        count = x

count = count + 1
cam = Camera()
for i in range(NUM):
    im = cam.getImage()
    #current_time = strftime("%Y-%m-%d %H:%M:%S", gmtime())
    filename = 'input_images/P%i-%05d.jpg' % (pitchnum, count)
    im.save(filename) 
    print filename,' saved'
    count = count + 1
    time.sleep(SLEEP_TIME)


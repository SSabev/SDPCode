#Call this script by: python test.py pitchnum
from SimpleCV import *
from features import Features
from threshold import Threshold
from display import Gui
from preprocess import Preprocessor
import os
import sys
import time
from time import gmtime, strftime
import util
#import SimpleCV.Color




def process_frame(frame, filename):
    #frame = preprocessor.preprocess(frame)
    ents = features.extractFeatures(frame)
    OK = True
    
    for name in ['yellow', 'blue', 'ball']:
        angle_rad = ents[name].angle()
        angle = 360 - (((angle_rad * (180 / math.pi)) - 360) % 360)
        
        x, y = ents[name].coordinates()
        #print name, x, y, angle
        
        #Draw entity to the image
        frame.drawCircle( (x,y), 5, Color.GREEN, -1)
        if name != 'ball':
            xx = x + 50*math.cos(angle_rad)
            yy = y + 50*math.sin(angle_rad)
            frame.drawLine( (x,y), (xx,yy), Color.BLACK, 1)
            #frame.drawCircle( (xx,yy), 3, Color.BLUE, 3)
        
        #Update statistics
        if x!=-1 and y != -1:
            recog[name] = recog[name] + 1
        else:
            OK = False
        
        full_path_output = os.path.join (OUTPUT_DIR, filename)
        frame.save(full_path_output)
    
    return OK

# Main program

if len(sys.argv) > 1:
    pitchnum = int (sys.argv[1])
else:
    pitchnum = 0

threshold = Threshold(pitchnum)
gui = Gui(True)
features = Features(gui, threshold)
#preprocessor = Preprocessor(False)
INPUT_DIR = './input_images'
OUTPUT_DIR = './output_images'
error_list = []

# Statistics
recog = {'ball' : 0, 'blue' : 0, 'yellow' : 0}
n_files = 0

# Process all images in IMPUT_DIR
for filename in os.listdir(INPUT_DIR):
    if filename[1] == str(pitchnum):
        n_files = n_files + 1
        full_path = os.path.join(INPUT_DIR, filename)
        frame = Image(full_path)
        print 'File %s processed\n' % filename
        if not process_frame(frame, filename):
            error_list.append(filename)
    
# Output the report
f = open('test_report.txt', 'a')
current_time = strftime("%Y-%m-%d %H:%M:%S", gmtime())
f.write('---------------------------------------------------------------------\n\n')
f.write(current_time)
f.write('\n')
f.write('Results of test on pitch %i \n' % pitchnum)

thres_path = os.path.join('data', 'threshdefaults_%i' % pitchnum)
th = util.loadFromFile(thres_path)
f.write('The current thresholds are:\n')
f.write(str(th) + '\n\n')

f.write('Percent recognized (not percent correct): \n')

if n_files > 0:
    f.write('Yellow: \t %i/%i \t %f percents \n' % (recog['yellow'], n_files, recog['yellow'] *100.0/n_files))
    f.write('Blue:   \t %i/%i \t %f percents \n' % (recog['blue'],   n_files, recog['blue']   *100.0/n_files))
    f.write('Ball:   \t %i/%i \t %f percents \n' % (recog['ball'],   n_files, recog['ball']   *100.0/n_files))


f.write('\nList of files with objects not recognized:\n')
f.write(str(error_list))
f.write('\n\n')
f.closed


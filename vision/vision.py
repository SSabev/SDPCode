from __future__ import print_function
import time
import os
import sys
import math

from SimpleCV import Camera
from preprocess import Preprocessor
from features import Features
from threshold import Threshold
from display import Gui
from threshold_gui import ThresholdGui
from debug_window import DebugWindow

from c_types import *


# PITCH_SIZE = (243.8, 121.9)


class Vision:

    def __init__(self, pitchnum, stdout, sourcefile, resetPitchSize, noGui, debug_window,  pipe):

        self.noGui = noGui
        self.lastFrameTime = self.begin_time = time.time()
        self.processed_frames = 0

        self.running = True
        self.stdout = stdout

        self.pipe = pipe
        

        
        if sourcefile is None:
            self.camera = Camera()
        else:
            self.filetype = 'video'
            if sourcefile.endswith(('jpg', 'png')):
                self.filetype = 'image'

        self.gui = Gui(self.noGui)
        self.threshold = Threshold(pitchnum)
        self.thresholdGui = ThresholdGui(self.threshold, self.gui)
        self.preprocessor = Preprocessor(resetPitchSize)
        self.features = Features(self.gui, self.threshold)
        # if self.debug_window:
        #     self.debug_window = DebugWindow()
        # else:
        #     self.debug_window = None

        calibrationPath = os.path.join('calibration', 'pitch{0}'.format(pitchnum))
        self.camera.loadCalibration(os.path.join(sys.path[0], calibrationPath))

        eventHandler = self.gui.getEventHandler()
        eventHandler.addListener('q', self.quit)

        # Ugly stuff for smoothing coordinates - should probably move it
        self._pastSize = 5
        self._pastCoordinates = {
                            'yellow': [(0, 0)] * self._pastSize,
                            'blue': [(0, 0)] * self._pastSize,
                            'ball': [(0, 0)] * self._pastSize
                            }
        self._pastAngles = {
                            'yellow': [1.0] * self._pastSize,
                            'blue': [1.0] * self._pastSize
                           }

        while self.running:
            if self.preprocessor.hasPitchSize:
                self.outputPitchSize()
                self.gui.setShowMouse(False)
            else:
                eventHandler.setClickListener(self.setNextPitchCorner)

            while self.running:
                self.doStuff()

    def quit(self):
        self.running = False
        self.pipe.send('q')

    def print_fps(self):
        thisFrameTime = time.time()
        time_diff = thisFrameTime - self.lastFrameTime
        fps = 1.0 / time_diff
        self.processed_frames = self.processed_frames + 1
        avg_fps = self.processed_frames * 1.0 / (thisFrameTime - self.begin_time)
        self.lastFrameTime = thisFrameTime

        if self.stdout:
            print("Instantaneous fps = %f Average fps = %f" % (fps, avg_fps))

    def doStuff(self):

        frame = self.camera.getImageUndistort()

        # Uncomment to see changes in barrell distortion matrix
        # calibrationPath = os.path.join('calibration', 'pitch{0}'.format(0))
        # self.camera.loadCalibration(os.path.join(sys.path[0], calibrationPath))

        frame = self.preprocessor.preprocess(frame)

        self.gui.updateLayer('raw', frame)

        ents = self.features.extractFeatures(frame)
        self.outputEnts(ents)

        self.print_fps()

        self.gui.loop()

    def setNextPitchCorner(self, where):
        self.preprocessor.setNextPitchCorner(where)

        if self.preprocessor.hasPitchSize:
            self.outputPitchSize()
            self.gui.setShowMouse(False)
            self.gui.updateLayer('corner', None)
        else:
            self.gui.drawCrosshair(where, 'corner')

    def outputPitchSize(self):
        if self.stdout:
            print ("Pitch size:\t %i\t %i\n" % tuple(self.preprocessor.pitch_size))
        # if self.debug_window:
        #     self.debug_window.insert_text("Pitch size:\t %i\t %i\n" % tuple(self.preprocessor.pitch_size))
        self.pipe.send(InitSignal(self.preprocessor.pitch_size[0], self.preprocessor.pitch_size[1]))

    def addCoordinates(self, entity, coordinates):
	self._pastCoordinates[entity].pop(0)
        self._pastCoordinates[entity].append(coordinates)
	
        #(x, y) = coordinates;
        
        # if the frame is bad(-1) then add the most recent coordinate instead
        #if (x != -1):
        #    self._pastCoordinates[entity].append(coordinates)
        #else:
        #    self._pastCoordinates[entity].append(self._pastCoordinates[entity][-1])

    def smoothCoordinates(self, entity):
        x = sum(map(lambda (x, _): x, self._pastCoordinates[entity])) / self._pastSize
        y = sum(map(lambda (_, y): y, self._pastCoordinates[entity])) / self._pastSize
        return (x, y)

    def addAngle(self, entity, angle):
        self._pastAngles[entity].pop(0)
        self._pastAngles[entity].append(angle)
        # if the frame is bad(-1) then add the most recent angle instead
        # good angle is always in (0,2pi), bad angle is -1, careful with real number
        #if (angle > -0.5):
        #    self._pastAngles[entity].append(angle)
        #else:
        #    self._pastAngles[entity].append(self._pastAngles[entity][-1])


    def smoothAngle(self, entity):
        # angle is periodic (of 2pi) and a bit tricky to smooth
        temp = sorted (self._pastAngles[entity])
        
        # if max_angle > min_angle > pi, those angles are crossing 0
        # we must add a period to the small ones
        if (temp[-1] - temp[0] > math.pi):
            temp = map(lambda angle: angle + 2*math.pi if angle < math.pi else angle, temp)
        
        return sum(temp) / self._pastSize
        
    # add/substract period (2pi) so angle is always in (0,2pi)
    # assume they are off by at most a period
    def standardize_angle(self, angle):
        if (angle > 2*math.pi):
            return angle - 2*math.pi
        if (angle < 0):
            return angle + 2*math.pi
        return angle
            
    def outputEnts(self, ents):
        # Messyyy
        if not self.preprocessor.hasPitchSize:
            return

        msg_data = []

        for name in ['yellow', 'blue', 'ball']:
            entity = ents[name]
            coordinates = entity.coordinates()
            
            # This is currently not needed
            # if the frame is not recognized, skip a maximum of self.max_skip times
            #if (coordinates[0] != -1):
            #    self.addCoordinates(name, coordinates)
            #    self.skip = 0
            #else:
            #    if (self.skip < self.max_skip):
            #        self.skip = self.skip + 1;
            #    else:
            #        self.addCoordinates(name, coordinates)
                    
            self.addCoordinates(name, coordinates)
            x, y = self.smoothCoordinates(name)

            # TODO: The system needs (0, 0) at top left!
            if y != -1:
                y = self.preprocessor.pitch_size[1] - y

            if name == 'ball':
                # self.send('{0} {1} '.format(x, y))
                msg_data += [int(x), int(y)]
                #print (self._pastCoordinates[name])
                #print(coordinates)
            else:
                # angle is currently clockwise, this makes it anti-clockwise
                angle = self.standardize_angle( 2*math.pi - entity.angle() )
                
                self.addAngle(name, angle)
                angle = self.standardize_angle ( self.smoothAngle(name) );
                
                msg_data += [int(x), int(y), angle]
                


        msg_data.append(int(time.time() * 1000))
        data = FrameData(*msg_data)

        if self.stdout:
            print ("Yellow:\t %i\t %i\t Angle:\t %s\nBlue:\t %i\t %i\t Angle:\t %s\nBall:\t %i\t %i\t\nTime:\t %i\n" % tuple(msg_data))
        # if debug_window:
        #     debug_window.insert_text("Yellow:\t %i\t %i\t Angle:\t %s\nBlue:\t %i\t %i\t Angle:\t %s\nBall:\t %i\t %i\t\nTime:\t %i\n" % tuple(msg_data))

        self.pipe.send(data)

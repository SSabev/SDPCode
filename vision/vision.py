from __future__ import print_function
import time
import os
import sys

from SimpleCV import Camera
from preprocess import Preprocessor
from features import Features
from threshold import Threshold
from display import Gui, ThresholdGui

from c_types import *


PITCH_SIZE = (243.8, 121.9)


class Vision:

    def __init__(self, pitchnum, stdout, sourcefile, resetPitchSize, noGui, pipe):

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

        calibrationPath = os.path.join('calibration', 'pitch{0}'.format(pitchnum))
        self.camera.loadCalibration(os.path.join(sys.path[0], calibrationPath))

        eventHandler = self.gui.getEventHandler()
        eventHandler.addListener('q', self.quit)

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
        self.pipe.send(InitSignal(self.preprocessor.pitch_size[0], self.preprocessor.pitch_size[1]))

    def outputEnts(self, ents):

        # Messyyy
        if not self.preprocessor.hasPitchSize:
            return

        msg_data = []

        for name in ['yellow', 'blue', 'ball']:
            entity = ents[name]
            x, y = entity.coordinates()

            # TODO: The system needs (0, 0) at top left!
            if y != -1:
                y = self.preprocessor.pitch_size[1] - y

            if name == 'ball':
                # self.send('{0} {1} '.format(x, y))
                msg_data += [int(x), int(y)]
            else:
                msg_data += [int(x), int(y), entity.angle()]

        msg_data.append(int(time.time() * 1000))
        data = FrameData(*msg_data)

        if self.stdout:
            print ("Yellow:\t %i\t %i\t Angle:\t %s\nBlue:\t %i\t %i\t Angle:\t %s\nBall:\t %i\t %i\t\nTime:\t %i\n" % tuple(msg_data))

        self.pipe.send(data)

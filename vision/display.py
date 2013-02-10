import pygame
import time
import cv
from SimpleCV import Display, DrawingLayer


class Gui:

    """
    Default layer groups available to display
    First element is the 'base' layer, and must be an image,
    the rest must be features and will be drawn on top
    """
    layersets = {
            'default': ['raw', 'yellow', 'blue', 'ball'],
            'yellow': ['threshY', 'yellow'],
            'blue': ['threshB', 'blue'],
            'ball': ['threshR', 'ball']
            }

    def __init__(self, noGui):
        self._layers = {
                # Base layers
                'raw': None,
                'threshY': None,
                'threshB': None,
                'threshR': None,

                # Overlay layers
                'yellow': None,
                'blue': None,
                'ball': None,
                }

        # These layers are drawn regardless of the current layerset
        self._persistentLayers = {
                'mouse': None
        }

        self._currentLayerset = self.layersets['default']
        self._display = Display()
        self._eventHandler = Gui.EventHandler()
        self._lastMouseState = 0
        self._showMouse = True
        self._lastFrame = None
        self._lastFrameTime = time.time()

    def __draw(self):

        iterator = iter(self._currentLayerset)

        # First element is the base layer
        baseLayer = self._layers[iterator.next()]

        if baseLayer is None:
            return

        # Draw all entities to one layer (for speed)
        entityLayer = baseLayer.dl()
        for key in iterator:
            toDraw = self._layers[key]
            if toDraw is None:
                continue

            elif isinstance(toDraw, DrawingLayer):
                baseLayer.addDrawingLayer(toDraw)

            else:
                toDraw.draw(entityLayer)

        for layer in self._persistentLayers.itervalues():
            if layer is not None:
                baseLayer.addDrawingLayer(layer)

        finalImage = baseLayer.applyLayers()
        self._display.writeFrame(finalImage, fit=False)

    def __updateFps(self):
        smoothConst = 0.1
        thisFrameTime = time.time()

        thisFrame = thisFrameTime - self._lastFrameTime
        if self._lastFrame is not None:
            # Smooth values
            thisFrame = thisFrame * (1 - smoothConst) + smoothConst * self._lastFrame

        self._lastFrame = thisFrame
        self._lastFrameTime = thisFrameTime

    def drawCrosshair(self, pos, layerName=None):
        size = self._layers['raw'].size()
        if layerName is not None:
            layer = self.getDrawingLayer()
        else:
            layer = self._layers['raw'].dl()

        layer.line((0, pos[1]), (size[0], pos[1]), color=(0, 0, 255))
        layer.line((pos[0], 0), (pos[0], size[1]), color=(0, 0, 255))

        if layerName is not None:
            self.updateLayer(layerName, layer)

    def loop(self):
        """
        Draw the image to the display, and process any events
        """

        for event in pygame.event.get(pygame.KEYDOWN):
            self._eventHandler.processKey(chr(event.key % 0x100))

        self._display.checkEvents()
        mouseX = self._display.mouseX
        mouseY = self._display.mouseY

        if self._showMouse:
            self.drawCrosshair((mouseX, mouseY), 'mouse')

        mouseLeft = self._display.mouseLeft
        # Only fire click event once for each click
        if mouseLeft == 1 and self._lastMouseState == 0:
            self._eventHandler.processClick((mouseX, mouseY))

        self._lastMouseState = mouseLeft

        # Processing OpenCV events requires calling cv.WaitKey() with a reasonable timeout,
        # which hits our framerate hard (NOTE: Need to confirm this on DICE), so only do
        # this if the focus isn't on the pygame (image) window`
        if not pygame.key.get_focused():
            c = cv.WaitKey(2)
            self._eventHandler.processKey(chr(c % 0x100))

        if self.showPixel:
            self.__updatePixel()
        self.__updateFps()
        self.__draw()

    def getEventHandler(self):
        return self._eventHandler

    def getDrawingLayer(self):
        return DrawingLayer(self._layers['raw'].dl())

    def updateLayer(self, name, layer):
        """
        Update the layer specified by 'name'
        If the layer name is not in the known list of layers,
        then it will be drawn regardless of the current view setting
        """

        if name in self._layers.keys():
            self._layers[name] = layer
        else:
            self._persistentLayers[name] = layer

    def switchLayerset(self, name):
        assert name in self.layersets.keys(), 'Unknown layerset ' + name + '!'

        self._currentLayerset = self.layersets[name]

    def setShowMouse(self, showMouse):
        if not showMouse:
            self.updateLayer('mouse', None)

        self._showMouse = showMouse

    # Show pixel mode: turn on/off by 'p', show the HSV of the pixel at the mouse
    showPixel = False

    def toggleShowPixel(self):
        self.showPixel = not self.showPixel
    # Record pixel mode: use the current pixel to update threshold
    recordPixel = False

    def toggleRecordPixel(self):
        self.recordPixel = not self.recordPixel

    def getMousePos(self):
        return (self._display.mouseX, self._display.mouseY)

    def getPixelHSV(self, x, y):
        return self._layers['raw'].crop(x, y, 1, 1).toHSV()[0, 0]

    def getPixel(self, x, y):
        return self._layers['raw'][x, y]

    #check (x,y) is in the image range
    def inRange(self, x, y):
        h = self._layers['raw'].height
        w = self._layers['raw'].width
        return (0 < x and x < w and 0 < y and y < h)

    # Display the pixel HSV
    def __updatePixel(self):
        (x, y) = self.getMousePos()
        if self.inRange(x, y):
            (h, s, v) = self.getPixelHSV(x, y)
        else:
            return

        if self.recordPixel:
            self.updateMinMax(h, s, v)

        drawingLayer = self.getDrawingLayer()
        drawingLayer.text('Pixel ({0}, {1}) HSV = ({2}, {3}, {4})'.format(x,y,h,s,v),(10,10), fgcolor(255, 255, 255))
        # drawingLayer.ezViewText('HSV_min =  ({0}, {1}, {2}) '.format(self.h_min, self.s_min, self.v_min),(10,30))
        # drawingLayer.ezViewText('HSV_max =  ({0}, {1}, {2}) '.format(self.h_max, self.s_max, self.v_max),(10,50))
        print ('Pixel ({0}, {1}) HSV = ({2}, {3}, {4})'.format(x, y, h, s, v))
        print ('HSV_min =  ({0}, {1}, {2}) '.format(self.h_min, self.s_min, self.v_min))
        print ('HSV_max =  ({0}, {1}, {2}) '.format(self.h_max, self.s_max, self.v_max))
        if self.recordPixel:
            print 'Recording Pixel'
            #drawingLayer.ezViewText('Recording Pixel',(10,70))

        #print 'pixel ',(x,y), ' HSV = ', (h,s,v), ' RGB = ', (r,g,b),
        #print 'HSV_min = ', (self.h_min, self.s_min, self.v_min),
        #print 'HSV_max = ', (self.h_max, self.s_max, self.v_max)

    def resetMinMax(self):
        self.h_min = self.s_min = self.v_min = 255
        self.h_max = self.s_max = self.v_max = 0

    # use the given HSV to update threshold
    def updateMinMax(self, h, s, v):
        self.h_min = min(self.h_min, h)
        self.s_min = min(self.s_min, s)
        self.v_min = min(self.v_min, v)
        self.h_max = max(self.h_max, h)
        self.s_max = max(self.s_max, s)
        self.v_max = max(self.v_max, v)

    class EventHandler:

        def __init__(self):
            self._listeners = {}
            self._clickListener = None

        def processKey(self, key):
            if key in self._listeners.keys():
                self._listeners[key]()

        def processClick(self, where):
            if self._clickListener is not None:
                self._clickListener(where)

        def addListener(self, key, callback):
            """
            Adds a function callback for a key.
            """
            assert callable(callback), '"callback" must be callable'

            self._listeners[key] = callback

        def setClickListener(self, callback):
            """
            Sets a function to be called on clicking on the image.
            The function will be passed a tuple with the (x,y) of the click.

            Setting a new callback will override the last one (or pass None to clear)
            """
            assert callback is None or callable(callback), '"callback" must be callable'

            self._clickListener = callback

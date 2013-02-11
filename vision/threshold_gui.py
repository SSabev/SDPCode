import cv


class ThresholdGui:

    def __init__(self, thresholdinstance, gui, window=None):

        if window is None:
            self.window = 'thresh_adjust'
            cv.NamedWindow(self.window, 0)
        else:
            self.window = window

        self._gui = gui
        self.threshold = thresholdinstance

        self._showOnGui = False

        self.__createTrackbars()
        self.__setupKeyEvents()

        self.changeEntity('yellow')

    def __setupKeyEvents(self):
        """
        Adds key listeners to the main gui for switching between entities
        """

        def yellow(): self.changeEntity('yellow')
        def blue(): self.changeEntity('blue')
        def ball(): self.changeEntity('ball')

        keyHandler = self._gui.getEventHandler()
        keyHandler.addListener('y', yellow)
        keyHandler.addListener('b', blue)
        keyHandler.addListener('r', ball)

        keyHandler.addListener('t', self.toggleShowOnGui)
        keyHandler.addListener('p', self.toggleShowPixel)
        keyHandler.addListener('o', self.toggleRecordPixel)
        keyHandler.addListener('i', self.applyMinMax)
        keyHandler.addListener('u', self.resetMinMax)

    def applyMinMax(self):
        h_min = int(self._gui.h_min)
        s_min = int(self._gui.s_min)
        v_min = int(self._gui.v_min)
        h_max = int(self._gui.h_max)
        s_max = int(self._gui.s_max)
        v_max = int(self._gui.v_max)

        allvalues = [[h_min, s_min, v_min], [h_max, s_max, v_max]]
        # print allvalues
        self.threshold.updateValues(self.currentEntity, allvalues)
        name = self.currentEntity
        self.setTrackbarValues(self.threshold._values[name])

        # Make sure trackbars update immediately
        cv.WaitKey(2)

        if self._showOnGui:
            self._gui.switchLayerset(name)

    def resetMinMax(self):
        self._gui.resetMinMax()

    def __createTrackbars(self):

        cv.CreateTrackbar('H min', self.window, 0, 255, self.__onTrackbarChanged)
        cv.CreateTrackbar('S min', self.window, 0, 255, self.__onTrackbarChanged)
        cv.CreateTrackbar('V min', self.window, 0, 255, self.__onTrackbarChanged)

        cv.CreateTrackbar('H max', self.window, 0, 255, self.__onTrackbarChanged)
        cv.CreateTrackbar('S max', self.window, 0, 255, self.__onTrackbarChanged)
        cv.CreateTrackbar('V max', self.window, 0, 255, self.__onTrackbarChanged)

    def __onTrackbarChanged(self, x):

        allvalues = []
        for which in ['min', 'max']:
            values = []
            for channel in ['H', 'S', 'V']:
                pos = cv.GetTrackbarPos('{0} {1}'.format(channel, which), \
                        self.window)

                values.append(pos)

            allvalues.append(values)

        self.threshold.updateValues(self.currentEntity, allvalues)

    def toggleShowOnGui(self):
        self._showOnGui = not self._showOnGui

        if self._showOnGui:
            self._gui.switchLayerset(self.currentEntity)
        else:
            self._gui.switchLayerset('default')

    def toggleShowPixel(self):
        self._gui.toggleShowPixel()

    def toggleRecordPixel(self):
        self._gui.toggleRecordPixel()

    def changeEntity(self, name):
        """
        Change which entity to adjust thresholding
        Can be 'blue', 'yellow' or 'ball'
        """

        self.currentEntity = name
        self.setTrackbarValues(self.threshold._values[name])

        # Make sure trackbars update immediately
        cv.WaitKey(2)

        if self._showOnGui:
            self._gui.switchLayerset(name)

    def setTrackbarValues(self, values):
        for i, which in enumerate(['min', 'max']):
            for j, channel in enumerate(['H', 'S', 'V']):
                cv.SetTrackbarPos('{0} {1}'.format(channel, which), \
                        self.window, values[i][j])

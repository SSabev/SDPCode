#DEFINES += ARDUINO_BLD
DEFINES += NXT_BUILD

#DEFINES += BUILDING_ON_DICE

CONFIG += network
QT += network

RESOURCES += \
    GUI/rc.qrc

HEADERS += \
    GUI/MainWindow.h \
    GUI/Tools/TeamCfgDlg.h \
    GUI/Comm/VisionComm.h \
    AI/AIControl.h \
    AI/AStar/AStarNode.h \
    AI/AStar/AStar.h \
    AI/Eagle/Eagle.h \
    AI/Foresee/Foresee.h \
    AI/Impala/Impala.h \
    AI/MathTypes/Vector2.h \
    AI/RobotState.h \
    Shared/Sockets.h \
    Shared/SharedMem.h \
    Shared/Logging.h \
    Navigation/Navigation.h \
    GUI/Tools/LoggingWdgt.h \
    GUI/Tools/CVisionMod.h \
    GUI/Comm/IBTComm.h

SOURCES += \
    GUI/SharedMem.cpp \
    GUI/MainWindow.cpp \
    GUI/main.cpp \
    GUI/Logging.cpp \
    GUI/Tools/TeamCfgDlg.cpp \
    GUI/Comm/VisionComm.cpp \
    AI/AIControl.cpp \
    AI/AStar/AStarNode.cpp \
    AI/AStar/AStar.cpp \
    AI/Eagle/Eagle.cpp \
    AI/Foresee/Foresee.cpp \
    AI/Impala/Impala.cpp \
    AI/MathTypes/Vector2.cpp \
    AI/RobotState.cpp \
    Navigation/Navigation.cpp \
    GUI/Tools/LoggingWdgt.cpp \
    GUI/Tools/CVisionMod.cpp

FORMS += \
    GUI/mainwindow.ui \
    GUI/Tools/TeamCfgDlg.ui

INCLUDEPATH += \
    Shared \
    GUI/Tools \
    $$PWD/AI \
    $$PWD/Navigation


contains(DEFINES, ARDUINO_BLD) {
    HEADERS += \
        GUI/Comm/ArduinoComm.h \
        GUI/Comm/ThreadedClasses.h

    SOURCES += \
        GUI/Comm/ArduinoComm.cpp \
        GUI/Comm/ThreadedClasses.cpp

    LIBS += -lbluetooth

    contains(DEFINES, BUILDING_ON_DICE) {
        INCLUDEPATH += $$PWD/Bluez_DICE/include

        LIBS += -L$$PWD/Bluez_DICE/lib64
    }

    contains(DEFINES, NXT_BUILD) {
        error(There can be only one communicator)
    }
}

contains(DEFINES, NXT_BUILD) {
    HEADERS += \
        GUI/Comm/BTComm.h

    SOURCES += \
        GUI/Comm/BTComm.cpp

    contains(DEFINES, ARDUINO_BLD) {
        error(There can be only one communicator)
    }
}

HEADERS += \
    GUI/MainWindow.h \
    GUI/Tools/TeamCfgDlg.h \
    GUI/Comm/VisionComm.h \
    GUI/Comm/BTComm.h \
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
    GUI/Tools/LoggingWdgt.h

SOURCES += \
    GUI/SharedMem.cpp \
    GUI/MainWindow.cpp \
    GUI/main.cpp \
    GUI/Logging.cpp \
    GUI/Tools/TeamCfgDlg.cpp \
    GUI/Comm/VisionComm.cpp \
    GUI/Comm/BTComm.cpp \
    AI/AIControl.cpp \
    AI/AStar/AStarNode.cpp \
    AI/AStar/AStar.cpp \
    AI/Eagle/Eagle.cpp \
    AI/Foresee/Foresee.cpp \
    AI/Impala/Impala.cpp \
    AI/MathTypes/Vector2.cpp \
    AI/RobotState.cpp \
    Navigation/Navigation.cpp \
    GUI/Tools/LoggingWdgt.cpp

FORMS += \
    GUI/mainwindow.ui \
    GUI/Tools/TeamCfgDlg.ui

INCLUDEPATH += \
    Shared \
    $$PWD/AI \
    $$PWD/Navigation

CONFIG += network
QT += network

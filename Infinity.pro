HEADERS += \
    GUI/MainWindow.h \
    GUI/LoggingWidget.h \
    GUI/Tools/TeamCfgDlg.h \
    GUI/Comm/VisionComm.h \
    GUI/Comm/BTComm.h

SOURCES += \
    GUI/SharedMem.cpp \
    GUI/MainWindow.cpp \
    GUI/main.cpp \
    GUI/LoggingWidget.cpp \
    GUI/Logging.cpp \
    GUI/Tools/TeamCfgDlg.cpp \
    GUI/Comm/VisionComm.cpp \
    GUI/Comm/BTComm.cpp

FORMS += \
    GUI/mainwindow.ui \
    GUI/Logging.ui \
    GUI/Tools/TeamCfgDlg.ui

INCLUDEPATH += \
    Shared

CONFIG += network
QT += network

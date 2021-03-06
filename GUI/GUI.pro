FORMS += \
    mainwindow.ui \
    Tools/TeamCfgDlg.ui

HEADERS += \
    MainWindow.h \
    ../Shared/SharedMem.h \
    ../Shared/Logging.h \
    ../Shared/Sockets.h \
    Comm/BTComm.h \
    Comm/VisionComm.h \
    Tools/TeamCfgDlg.h \
    Tools/LoggingWdgt.h

SOURCES += \
    MainWindow.cpp \
    main.cpp \
    SharedMem.cpp \
    Logging.cpp \
    Comm/BTComm.cpp \
    Comm/VisionComm.cpp \
    Tools/TeamCfgDlg.cpp \
    Tools/LoggingWdgt.cpp

INCLUDEPATH += \
        Shared \
        Tools/ \
        ${QMAKE_VAR__PRO_FILE_PWD_}../Shared

CONFIG += network
QT += network

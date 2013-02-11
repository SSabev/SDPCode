FORMS += \
    mainwindow.ui \
    Logging.ui \
    Tools/TeamCfgDlg.ui

HEADERS += \
    MainWindow.h \
    ../Shared/SharedMem.h \
    ../Shared/Logging.h \
    LoggingWidget.h \
    ../Shared/Sockets.h \
    Comm/BTComm.h \
    Comm/VisionComm.h \
    Tools/TeamCfgDlg.h \
    ../Navigation/Navigation.h

SOURCES += \
    MainWindow.cpp \
    main.cpp \
    SharedMem.cpp \
    LoggingWidget.cpp \
    Logging.cpp \
    Comm/BTComm.cpp \
    Comm/VisionComm.cpp \
    Tools/TeamCfgDlg.cpp \
    ../Navigation/Navigation.cpp

INCLUDEPATH += \
        Shared \
        ${QMAKE_VAR__PRO_FILE_PWD_}../Shared

CONFIG += network
QT += network

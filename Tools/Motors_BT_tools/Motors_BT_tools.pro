DEFINES += BUILDING_ON_DICE

FORMS += \
    MainWidget.ui

HEADERS += \
    MainWidget.h \
    ThreadedClasses.h \
    KeysWidget.h

SOURCES += \
    MainWidget.cpp \
    ThreadedClasses.cpp \
    main.cpp \
    KeysWidget.cpp

LIBS += -lbluetooth

contains(DEFINES, BUILDING_ON_DICE) {
INCLUDEPATH += $$PWD/../../Bluez_DICE/include

LIBS += -L$$PWD/../../Bluez_DICE/lib64
}

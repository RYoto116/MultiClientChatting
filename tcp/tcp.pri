QT += network
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/forwardthread.h \
    $$PWD/header.h \
    $$PWD/loginrunnable.h \
    $$PWD/registerrunnable.h \
    $$PWD/tcpconnection.h \
    $$PWD/tcpconnections.h \
    $$PWD/tcpserver.h

SOURCES += \
    $$PWD/forwardthread.cpp \
    $$PWD/loginrunnable.cpp \
    $$PWD/registerrunnable.cpp \
    $$PWD/tcpconnection.cpp \
    $$PWD/tcpconnections.cpp \
    $$PWD/tcpserver.cpp

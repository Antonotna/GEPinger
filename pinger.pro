#-------------------------------------------------
#
# Project created by QtCreator 2012-11-23T18:30:16
#
#-------------------------------------------------

QT       += core gui

TARGET = pinger
TEMPLATE = app

RC_FILE = addIcon.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    sender.cpp

HEADERS  += mainwindow.h \
    sender.h \
    StructPacket.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:/WpdPack/Include
LIBS += -L C:/WpdPack/Lib -lwpcap -lpacket
LIBS += C:/QtSDK/mingw/lib/libws2_32.a
LIBS += C:/QtSDK/mingw/lib/libIphlpapi.a

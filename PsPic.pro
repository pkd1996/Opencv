#-------------------------------------------------
#
# Project created by QtCreator 2019-08-28T09:36:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PsPic
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lgdi32 -luser32
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += D:\OpencvEnvironment\opencv\build\include\opencv \
               D:\OpencvEnvironment\opencv\build\include\opencv2 \
               D:\OpencvEnvironment\opencv\build\include

LIBS += D:\OpencvEnvironment\opencv\build_QT\lib\libopencv_*.a

SOURCES += \
        main.cpp \
        widget.cpp \
    formcalibration.cpp \
    puttext.cpp \
    textwidget.cpp

HEADERS += \
        widget.h \
    formcalibration.h \
    puttext.h \
    textwidget.h

FORMS += \
        widget.ui \
    formcalibration.ui

RESOURCES += \
    image.qrc

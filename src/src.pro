
TEMPLATE = app

TARGET = todoread

SOURCES += main.cpp \
    todoread.cpp \
    controller.cpp \
    downloadthread.cpp

HEADERS += \
    todoread.h \
    globals.h \
    controller.h \
    downloadthread.h

QT += gui

CONFIG += link_pkgconfig qdeclarative-boostable qt-components

contains(MEEGO_EDITION,harmattan):{
    PKGCONFIG += libsignon-qt
}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

target.path = /opt/todoread/bin
INSTALLS += target

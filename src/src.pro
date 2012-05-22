
TEMPLATE = app

TARGET = todoread

SOURCES += main.cpp \
    todoread.cpp \
    controller.cpp \
    downloadthread.cpp

contains(MEEGO_EDITION,harmattan):{
    SOURCES += identitymanager.cpp
}

HEADERS += \
    todoread.h \
    globals.h \
    controller.h \
    downloadthread.h

contains(MEEGO_EDITION,harmattan):{
    HEADERS += identitymanager.h
}

QT += gui webkit opengl

CONFIG += link_pkgconfig qdeclarative-boostable qt-components

contains(MEEGO_EDITION,harmattan):{
    PKGCONFIG += libsignon-qt accounts-qt AccountSetup
}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

target.path = /opt/todoread/bin
INSTALLS += target

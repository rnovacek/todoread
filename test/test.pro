
TEMPLATE = app

SOURCES += test.cpp #    ../readitlater/readitlater.cpp

HEADERS += test.h #    ../readitlater/readitlater.h

CONFIG += plugin qt debug signon-plugins link_pkgconfig

QT += network xml

contains(MEEGO_EDITION,harmattan):{
    PKGCONFIG += signon-qt signon-plugins
}

INCLUDEPATH += \#/home/radek/QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/include/signon-plugins/ \
 \#/home/radek/QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/include/signon-qt/ \
../readitlater/

LIBS += -L../readitlater #-lreaditlaterplugin

contains(MEEGO_EDITION,harmattan):{
    target.path = /opt/todoread/bin
    INSTALLS += target
}

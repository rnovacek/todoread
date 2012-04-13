
TEMPLATE = lib

TARGET = readitlaterplugin

CONFIG += plugin qt debug signon-plugins link_pkgconfig

QT += network

PKGCONFIG += signon-plugins

HEADERS += readitlaterprovider.h

SOURCES += readitlaterprovider.cpp

provider.files = readitlater.provider
provider.path = /usr/share/accounts/providers

icon.files = icon-m-service-readitlater.png
icon.path = /usr/share/themes/blanco/meegotouch/icons

target.path = /usr/lib/signon/

INSTALLS += target provider icon

OTHER_FILES += readitlater.provider

INCLUDEPATH += /home/radek/QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/include/signon-plugins/

QMAKE_CXXFLAGS += -fno-rtti

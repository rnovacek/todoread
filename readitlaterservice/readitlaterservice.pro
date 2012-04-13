
TEMPLATE = app

TARGET = readitlater

CONFIG += plugin qt debug webupload-plugin link_pkgconfig

QT += network

PKGCONFIG += accounts-qt libsignon-qt

HEADERS += readitlaterservice.h \
    readitlaterpost.h \
    readitlaterauth.h

SOURCES += readitlaterservice.cpp \
    readitlaterpost.cpp \
    readitlaterauth.cpp

service.files = readitlater.service
service.path = /usr/share/accounts/services

target.path = /usr/lib/webupload/plugins/

INSTALLS += target service

OTHER_FILES += readitlater.service


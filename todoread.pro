
TEMPLATE = subdirs

SUBDIRS = src

contains(MEEGO_EDITION,harmattan):{
    SUBDIRS += \
        readitlaterprovider \
        readitlaterservice
}

qml.files = qml/*
qml.path = /opt/todoread/qml

desktop.files = todoread.desktop
desktop.path = /usr/share/applications/

icon.files = todoread80.png
icon.path = /usr/share/icons/hicolor/80x80/apps/

splash.files = splash.png
splash.path = /opt/todoread/bin/

symbian:TARGET.UID3 = 0xE2584B99

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/todoread.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/*

INSTALLS += qml desktop icon splash

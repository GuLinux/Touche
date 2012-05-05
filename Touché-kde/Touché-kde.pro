#-------------------------------------------------
#
# Project created by QtCreator 2012-05-02T15:17:10
#
#-------------------------------------------------

QT       += core gui

TARGET = Touché
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=


CONFIG(debug, debug|release)   { COMPILE_MODE='debug' }
   else                        { COMPILE_MODE='release' }

LIBS += -L/tmp/Touché_Core/$$COMPILE_MODE -lTouché_Core
LIBS += -lqjson -lX11 -lXtst -lkdeui
QMAKE_CXXFLAGS += "-std=c++0x"

INCLUDEPATH += ../Touché_Core


target.path=/usr/bin
udev_rules.files=files/99-hiddev-input.rules
udev_rules.path=/etc/udev/rules.d

keyboard_database.files=files/keyboard_database.json
keyboard_database.path=/usr/share/$$TARGET/

desktop_entry.files += files/Touché.desktop
desktop_entry.path = /usr/share/applications/

INSTALLS += target udev_rules keyboard_database desktop_entry

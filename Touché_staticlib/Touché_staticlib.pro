#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T11:00:56
#
#-------------------------------------------------

QT       += core gui


TARGET = Touché_staticlib
TEMPLATE = lib
CONFIG += staticlib


SOURCES += backend/hiddev/hiddev.cpp \
    domain/dumpkeys.cpp \
    domain/translatekeyevents.cpp \
    domain/runcommandbinding.cpp \
    domain/tokeybinding.cpp \
    backend/hiddev/finddevices.cpp \
    domain/deviceinfo.cpp \
    backend/config/keyboarddatabase.cpp \
    backend/config/databaseentry.cpp \
    domain/inputevent.cpp \
    domain/inputregister.cpp \
    domain/configevent.cpp \
    backend/config/bindingsconfig.cpp \
    domain/donothingbinding.cpp \
    touchecore.cpp

HEADERS += \
    backend/hiddev/hiddev.h \
    domain/dumpkeys.h \
    domain/translatekeyevents.h \
    domain/binding.h \
    domain/runcommandbinding.h \
    domain/tokeybinding.h \
    backend/hiddev/finddevices.h \
    domain/deviceinfo.h \
    backend/config/keyboarddatabase.h \
    backend/config/databaseentry.h \
    domain/inputevent.h \
    domain/inputregister.h \
    domain/configevent.h \
    backend/config/bindingsconfig.h \
    domain/donothingbinding.h \
    touchecore.h

LIBS += -lqjson -lX11 -lXtst
QMAKE_CXXFLAGS += "-std=c++0x"

CONFIG(debug, debug|release)   { COMPILE_MODE='debug' }
   else                        { COMPILE_MODE='release' }

target.path=/tmp/$$TARGET/$$COMPILE_MODE
INSTALLS += target

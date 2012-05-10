#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T11:00:56
#
#-------------------------------------------------

QT       += core gui


TARGET = Touché_Core
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
    touchecore.cpp \
    domain/nomoreeventsinputevent.cpp \
    backend/updatekeysymbolmapping.cpp

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
    touchecore.h \
    domain/nomoreeventsinputevent.h \
    backend/updatekeysymbolmapping.h

LIBS += -lqjson -lX11 -lXtst
QMAKE_CXXFLAGS += "-std=c++0x"

CONFIG(debug, debug|release)   { COMPILE_MODE='debug' }
   else                        { COMPILE_MODE='release' }

isEmpty(TMP_INSTALL_DIR) {
     TMP_INSTALL_DIR="/tmp"
 }

target.path=$$TMP_INSTALL_DIR/$$TARGET/$$COMPILE_MODE
message("Will install static library to " $$target.path)

INSTALLS += target

QMAKE_POST_LINK += make install

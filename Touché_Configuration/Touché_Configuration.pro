#-------------------------------------------------
#
# Project created by QtCreator 2012-05-05T14:28:28
#
#-------------------------------------------------

TARGET = Touché_Configuration
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    keysconfigurationdialog.cpp \
    models/cfgdevice.cpp \
    models/cfgkey.cpp \
    models/cfgkeyevent.cpp \
    keybindingconfiguration.cpp \
    bindingconfigurationwidget.cpp \
    qstringlistedit.cpp \
    x11keysymbolscompleter.cpp

HEADERS += \
    keysconfigurationdialog.h \
    models/cfgdevice.h \
    models/cfgkey.h \
    models/cfgkeyevent.h \
    keybindingconfiguration.h \
    bindingconfigurationwidget.h \
    qstringlistedit.h \
    x11keysymbolscompleter.h

INCLUDEPATH += ../Touché_Core
LIBS += -L/tmp/Touché_Core/$$COMPILE_MODE -lTouché_Core

LIBS += -lqjson
QMAKE_CXXFLAGS += "-std=c++0x"

CONFIG(debug, debug|release)   { COMPILE_MODE='debug' }
   else                        { COMPILE_MODE='release' }

isEmpty(TMP_INSTALL_DIR) {
     TMP_INSTALL_DIR="/tmp"
 }

target.path=$$TMP_INSTALL_DIR/$$TARGET/$$COMPILE_MODE
INSTALLS += target

QMAKE_POST_LINK += make install

FORMS += \
    keysconfigurationdialog.ui \
    RunCommand_cfg.ui \
    ToKey_cfg.ui \
    keybindingconfiguration.ui \
    qstringlistedit.ui

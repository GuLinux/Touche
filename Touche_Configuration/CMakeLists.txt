set(Touche_Configuration_SRCS keysconfigurationdialog.cpp
    models/cfgdevice.cpp
    models/cfgkey.cpp
    models/cfgkeyevent.cpp
    keybindingconfiguration.cpp
    bindingconfigurationwidget.cpp
    qstringlistedit.cpp
    x11keysymbolscompleter.cpp)



set(Touche_Configuration_FORMS keysconfigurationdialog.ui
    RunCommand_cfg.ui
    ToKey_cfg.ui
    keybindingconfiguration.ui
    qstringlistedit.ui)

QT4_WRAP_UI(Touche_Configuration_FORMS_HEADERS ${Touche_Configuration_FORMS})
qt4_wrap_cpp(Touche_Configuration_mocs  bindingconfigurationwidget.h
  keybindingconfiguration.h
  keysconfigurationdialog.h
  models/cfgkeyevent.h
  models/cfgkey.h
  models/cfgdevice.h
  qstringlistedit.h
  x11keysymbolscompleter.h
)

set(Touche_Configuration_includes ${Touche_common_includes} )

include_directories(${Touche_Configuration_includes}) 
add_library(Touche_Configuration ${Touche_Configuration_SRCS} ${Touche_Configuration_FORMS_HEADERS} ${Touche_Configuration_mocs} )



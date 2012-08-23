set(Touche_Configuration_SRCS keysconfigurationdialog.cpp
    models/cfgdevice.cpp
    models/cfgkey.cpp
    models/cfgkeyevent.cpp
    keybindingconfiguration.cpp
    bindingconfigurationwidget.cpp
    bindingsGui/qstringlistedit.cpp
    bindingsGui/x11keysymbolscompleter.cpp
    bindingsGui/ToKeyConfig.cpp
    bindingsGui/RunCommandConfig.cpp
)



set(Touche_Configuration_FORMS keysconfigurationdialog.ui
    keybindingconfiguration.ui
    bindingsGui/RunCommand_cfg.ui
    bindingsGui/ToKey_cfg.ui
    bindingsGui/qstringlistedit.ui)

QT4_WRAP_UI(Touche_Configuration_FORMS_HEADERS ${Touche_Configuration_FORMS})
qt4_wrap_cpp(Touche_Configuration_mocs  bindingconfigurationwidget.h
  keybindingconfiguration.h
  keysconfigurationdialog.h
  models/cfgkeyevent.h
  models/cfgkey.h
  models/cfgdevice.h
  bindingsGui/qstringlistedit.h
  bindingsGui/x11keysymbolscompleter.h
  bindingsGui/ToKeyConfig.h
  bindingsGui/RunCommandConfig.h
)

set(Touche_Configuration_includes ${Touche_common_includes} )

include_directories(${Touche_Configuration_includes}) 
add_library(Touche_Configuration STATIC ${Touche_Configuration_SRCS} ${Touche_Configuration_FORMS_HEADERS} ${Touche_Configuration_mocs} )



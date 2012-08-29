set(BindingsConfigurationDialog_SRCS keysconfigurationdialog.cpp
    models/cfgdevice.cpp
    models/cfgkey.cpp
    models/cfgkeyevent.cpp
    keybindingconfiguration.cpp
    bindingconfigurationwidget.cpp
    bindingsGui/x11keysymbolscompleter.cpp
    bindingsGui/ToKeyConfig.cpp
    bindingsGui/RunCommandConfig.cpp
)



set(BindingsConfigurationDialog_FORMS keysconfigurationdialog.ui
    keybindingconfiguration.ui
    bindingsGui/RunCommand_cfg.ui
    bindingsGui/ToKey_cfg.ui
)

QT4_WRAP_UI(BindingsConfigurationDialog_FORMS_HEADERS ${BindingsConfigurationDialog_FORMS})
qt4_wrap_cpp(BindingsConfigurationDialog_mocs  bindingconfigurationwidget.h
  keybindingconfiguration.h
  keysconfigurationdialog.h
  models/cfgkeyevent.h
  models/cfgkey.h
  models/cfgdevice.h
  bindingsGui/x11keysymbolscompleter.h
  bindingsGui/ToKeyConfig.h
  bindingsGui/RunCommandConfig.h
)

set(BindingsConfigurationDialog_includes ${Touche_common_includes} )

include_directories(${BindingsConfigurationDialog_includes}) 
add_library(BindingsConfigurationDialog STATIC ${BindingsConfigurationDialog_SRCS} ${BindingsConfigurationDialog_FORMS_HEADERS} ${BindingsConfigurationDialog_mocs} )



set(Touche_GUI_SRCS 
  touchesystemtray.cpp
  EditProfiles.cpp
  SettingsDialog.cpp
  ModulesPage.cpp
)



# set(Touche_GUI_FORMS )

QT4_WRAP_UI(Touche_GUI_FORMS_HEADERS ModulesPage.ui)
qt4_wrap_cpp(Touche_GUI_mocs
  touchesystemtray.h
  EditProfiles.h
  SettingsDialog.h
  ModulesPage.h
)

include_directories( ${Touche_common_includes} ) 
add_library(Touche_GUI STATIC ${Touche_GUI_SRCS}  ${Touche_GUI_mocs} ${Touche_GUI_FORMS_HEADERS} )

add_dependencies(Touche_GUI Touche_Core Touche_Configuration) 



set(Touche_GUI_SRCS 
  touchesystemtray.cpp
)



# set(Touche_GUI_FORMS )

#QT4_WRAP_UI(Touche_GUI_FORMS_HEADERS ${Touche_GUI_FORMS})
qt4_wrap_cpp(Touche_GUI_mocs
  touchesystemtray.h
)

include_directories( ${Touche_common_includes} ) 
add_library(Touche_GUI STATIC ${Touche_GUI_SRCS}  ${Touche_GUI_mocs} )

add_dependencies(Touche_GUI Touche_Core Touche_Configuration) 



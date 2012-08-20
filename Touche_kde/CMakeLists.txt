find_package( KDE4 REQUIRED )
include(KDE4Defaults)
 
include_directories( ${KDE4_INCLUDES} )

set(Touche_kde_SRCS main.cpp
    trayIcon/touchesystemtray.cpp)

set(Touche_kde_includes ${Touche_common_includes}
  ${CMAKE_SOURCE_DIR}/Touche_kde ${CMAKE_BINARY_DIR}/Touche_kde
)

qt4_wrap_cpp(Touche_kde_mocs trayIcon/touchesystemtray.h)
include_directories(${Touche_kde_includes}) 
add_executable(Touche ${Touche_kde_SRCS} ${Touche_kde_mocs} )

target_link_libraries(Touche ${Touche_libs} ${KDE4_KDEUI_LIBS} )

# TODO for /etc/udev it would be better to use variables, although this one is not going to change that easly...
INSTALL(FILES files/99-hiddev-input.rules DESTINATION /etc/udev/rules.d)
INSTALL(FILES files/keyboard_database.json DESTINATION share/Touche)
INSTALL(FILES desktop/Touche.desktop DESTINATION share/applications)
INSTALL(TARGETS Touche RUNTIME DESTINATION bin)

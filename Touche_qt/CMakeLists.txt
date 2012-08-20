set(Touche_qt_SRCS main.cpp
    trayIcon/touchesystemtray.cpp)

set(Touche_qt_includes ${Touche_common_includes}
  ${CMAKE_SOURCE_DIR}/Touche_qt ${CMAKE_BINARY_DIR}/Touche_qt
)

qt4_wrap_cpp(Touche_qt_mocs trayIcon/touchesystemtray.h)
include_directories(${Touche_qt_includes}) 
add_executable(${PROJECT_NAME} ${Touche_qt_SRCS} ${Touche_qt_mocs} )

target_link_libraries(${PROJECT_NAME} ${Touche_libs} )

# TODO for /etc/udev it would be better to use variables, although this one is not going to change that easly...
INSTALL(FILES files/99-hiddev-input.rules DESTINATION /etc/udev/rules.d)
INSTALL(FILES files/keyboard_database.json DESTINATION share/${PROJECT_NAME})
INSTALL(FILES desktop/Touche.desktop DESTINATION share/applications)
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

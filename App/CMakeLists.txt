set(Touche_kde_SRCS main.cpp )

set(Touche_kde_includes ${Touche_common_includes}
  #${CMAKE_SOURCE_DIR}/Touche_kde ${CMAKE_BINARY_DIR}/Touche_kde
)

#qt4_wrap_cpp(Touche_kde_mocs trayIcon/touchesystemtray.h)
include_directories(${Touche_kde_includes}) 
add_executable(${PROJECT_NAME} ${Touche_kde_SRCS} ${Touche_kde_mocs} )

target_link_libraries(${PROJECT_NAME} ${Touche_libs} )
add_dependencies(${PROJECT_NAME} Touche_Core Touche_Configuration Touche_GUI) 

# TODO for /etc/udev it would be better to use variables, although this one is not going to change that easly...
INSTALL(FILES files/99-hiddev-input.rules DESTINATION /etc/udev/rules.d)
FILE(GLOB keyboardDatabases "files/KeyboardDatabases/*.json")
INSTALL(FILES ${keyboardDatabases} DESTINATION ${KDE4_DATA_INSTALL_DIR}/${PROJECT_NAME}/KeyboardDatabases/ )
INSTALL(FILES desktop/Touche.desktop DESTINATION share/applications)
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
KDE4_INSTALL_ICONS( ${ICON_INSTALL_DIR} )
install(FILES files/${PROJECT_NAME}.notifyrc DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME})

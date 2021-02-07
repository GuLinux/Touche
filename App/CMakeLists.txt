set(Touche_kde_SRCS main.cpp )

set(Touche_kde_includes ${Touche_common_includes}
  #${CMAKE_SOURCE_DIR}/Touche_kde ${CMAKE_BINARY_DIR}/Touche_kde
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fstack-protector-all")

#qt4_wrap_cpp(Touche_kde_mocs trayIcon/touchesystemtray.h)
include_directories(${Touche_kde_includes}) 
add_executable(${PROJECT_NAME} ${Touche_kde_SRCS} ${Touche_kde_mocs} )

target_link_libraries(${PROJECT_NAME} ${Touche_libs} )
add_dependencies(${PROJECT_NAME} Touche_Core Touche_GUI)

set(UDEV_RULES_PATH
    ${CMAKE_INSTALL_SYSCONFDIR}/udev/rules.d
    CACHE STRING
    "Target directory for udev rules")

INSTALL(FILES files/99-hiddev-input.rules DESTINATION ${UDEV_RULES_PATH})
FILE(GLOB keyboardDatabases "files/KeyboardDatabases/*.json")
INSTALL(FILES ${keyboardDatabases} DESTINATION ${Touche_keyboard_database_dir}/ )
INSTALL(FILES desktop/Touche.desktop DESTINATION share/applications)
INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
KDE4_INSTALL_ICONS( ${ICON_INSTALL_DIR} )
install(FILES files/${PROJECT_NAME}.notifyrc DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME})

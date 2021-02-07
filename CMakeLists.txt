cmake_minimum_required(VERSION 3.13)

file(READ "version.txt" Touche_VERSION)
project(Touche VERSION ${Touche_VERSION})

set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_PACKAGE_NAME "touche")
set(CPACK_PACKAGE_CONTACT "Eduardo Dobay <edudobay@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION "Touché is a KDE application that brings full support on GNU/Linux desktops to your input devices: keyboard, mice, even IR remote receivers")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Support for extra input devices: keyboard, mice, IR remote receivers")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.14), libkf5configwidgets5 (>= 4.96.0), libkf5coreaddons5 (>= 4.97.0), libkf5i18n5 (>= 4.97.0), libkf5kdelibs4support5 (>= 4.96.0), libkf5notifications5 (>= 4.96.0), libkf5widgetsaddons5 (>= 4.96.0), libkf5xmlgui-bin, libkf5xmlgui5 (>= 4.96.0), libqt5core5a (>= 5.12.2), libqt5gui5 (>= 5.7.0) | libqt5gui5-gles (>= 5.7.0), libqt5widgets5 (>= 5.0.2), libqt5x11extras5 (>= 5.6.0), libstdc++6 (>= 4.5), libx11-6, libxtst6")
set(CPACK_DEBIAN_PACKAGE_SUGGESTS "libqt5dbus5, libcwiid1")
set(CPACK_GENERATOR "DEB")

include(InstallRequiredSystemLibraries)
include(CPack)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)

find_package(Qt5Core REQUIRED COMPONENTS X11Extras)
find_package(Qt5Widgets REQUIRED)
find_package(KF5KDELibs4Support CONFIG REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
find_package(X11 REQUIRED)
find_package( CWiid )
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${KF5KDELibs4Support_INCLUDE_DIRS})


set(Touche_static_libraries
  ${CMAKE_BINARY_DIR}/Core/libTouche_Core.a
  ${CMAKE_BINARY_DIR}/GUI/libTouche_GUI.a
  ${CMAKE_BINARY_DIR}/BindingsConfigurationDialog/libBindingsConfigurationDialog.a
)

set(Touche_common_includes ${QT_INCLUDES}
  ${CMAKE_SOURCE_DIR}/Core ${CMAKE_BINARY_DIR}/Touche_Core
  ${CMAKE_SOURCE_DIR}/BindingsConfigurationDialog ${CMAKE_BINARY_DIR}/BindingsConfigurationDialog
  ${CMAKE_SOURCE_DIR}/GUI ${CMAKE_BINARY_DIR}/GUI
  ${CMAKE_SOURCE_DIR}/wiimote ${CMAKE_BINARY_DIR}/wiimote
)

set(Touche_keyboard_database_dir ${KF5_DATA_INSTALL_DIR}/${PROJECT_NAME}/KeyboardDatabases)
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")
add_compile_definitions(TOUCHE_APP_VERSION="${PROJECT_VERSION}")
add_compile_definitions(TOUCHE_KEYBOARD_DATABASE_DIR="${CMAKE_INSTALL_PREFIX}/${Touche_keyboard_database_dir}")
if(CWIID_FOUND)
    add_compile_definitions(HAVE_CWIID=1)
    set(Touche_libs ${Touche_libs} ${CWIID_LIBRARIES})
    set(Touche_static_libraries ${Touche_static_libraries} ${CMAKE_BINARY_DIR}/lib/libWiimoteBridge.a )
endif(CWIID_FOUND)

set(Touche_libs ${Touche_static_libraries} ${Touche_libs} ${X11_LIBRARIES} ${X11_XTest_LIB}
   Qt5::Widgets
   Qt5X11Extras
   KF5::KDELibs4Support
   )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-suggest-override -Wno-deprecated-declarations -std=c++11 -fPIC") # TODO: why fPIC is mandatory?


add_subdirectory(Core)
include_directories(Core)
add_subdirectory(BindingsConfigurationDialog)
add_subdirectory(GUI)
if(CWIID_FOUND)
    add_subdirectory(wiimote)
    add_compile_definitions(CWIID_FOUND=1)
endif(CWIID_FOUND)

add_subdirectory(App)

if(NOT CWIID_FOUND)
    message("Missing Optional dependencies:")
    message("  CWiid - for Wiimote support")
else(NOT CWIID_FOUND)
    message("Found Optional dependencies:")
    message("  CWiid - for Wiimote support")
endif(NOT CWIID_FOUND)

set(BUILD_TESTS Off CACHE BOOL "Enable building and running tests")
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif(BUILD_TESTS)


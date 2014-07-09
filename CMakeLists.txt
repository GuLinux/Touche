project(Touche)
cmake_minimum_required(VERSION 2.8.11)

#find_package(Qt5Core REQUIRED COMPONENTS X11Extras)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
find_package(X11 REQUIRED)
find_package( CWiid )
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#include_directories(${Qt5Widgets_INCLUDE_DIRS})


set(Touche_static_libraries
  ${CMAKE_BINARY_DIR}/lib/libTouche_Core.a
  ${CMAKE_BINARY_DIR}/lib/libTouche_GUI.a
  ${CMAKE_BINARY_DIR}/lib/libBindingsConfigurationDialog.a
)

set(Touche_common_includes ${QT_INCLUDES}
  ${CMAKE_SOURCE_DIR}/Core ${CMAKE_BINARY_DIR}/Touche_Core
  ${CMAKE_SOURCE_DIR}/BindingsConfigurationDialog ${CMAKE_BINARY_DIR}/BindingsConfigurationDialog
  ${CMAKE_SOURCE_DIR}/GUI ${CMAKE_BINARY_DIR}/GUI
  ${CMAKE_SOURCE_DIR}/wiimote ${CMAKE_BINARY_DIR}/wiimote
)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
if(CWIID_FOUND)
    add_definitions(-DHAVE_CWIID=1)
    set(Touche_libs ${Touche_libs} ${CWIID_LIBRARIES})
    set(Touche_static_libraries ${Touche_static_libraries} ${CMAKE_BINARY_DIR}/lib/libWiimoteBridge.a )
endif(CWIID_FOUND)

set(Touche_libs ${Touche_static_libraries} ${Touche_libs} ${X11_LIBRARIES} ${X11_XTest_LIB} Qt5::Widgets )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC") # TODO: why fPIC is mandatory?


add_subdirectory(Core)
include_directories(Core)
#add_subdirectory(BindingsConfigurationDialog)
#add_subdirectory(GUI)
if(CWIID_FOUND)
    add_subdirectory(wiimote)
endif(CWIID_FOUND)

#add_subdirectory(App)

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


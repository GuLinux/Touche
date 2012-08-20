project(Touche)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
find_package(QJSON REQUIRED)
find_package(X11 REQUIRED)

set(Touche_static_libraries
  ${CMAKE_BINARY_DIR}/Touche_Core/libTouche_Core.a
  ${CMAKE_BINARY_DIR}/Touche_Configuration/libTouche_Configuration.a
)

set(Touche_common_includes ${QT_INCLUDES}
  ${CMAKE_SOURCE_DIR}/Touche_Core ${CMAKE_BINARY_DIR}/Touche_Core
  ${CMAKE_SOURCE_DIR}/Touche_Configuration ${CMAKE_BINARY_DIR}/Touche_Configuration)


add_definitions(${QT_DEFINITIONS})
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
include(${QT_USE_FILE})

set(Touche_libs ${QT_LIBRARIES} ${Touche_static_libraries} ${X11_LIBRARIES} ${QJSON_LIBRARIES} ${X11_XTest_LIB} )


add_subdirectory(Touche_Core)
add_subdirectory(Touche_Configuration)

if(TOUCHE_QT)
  add_subdirectory(Touche_qt)
else(TOUCHE_QT)
  add_subdirectory(Touche_kde)
endif(TOUCHE_QT)

project(Touche)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
find_package(QJSON REQUIRED)
find_package(X11 REQUIRED)

set(Touche_static_libraries
  ${CMAKE_BINARY_DIR}/Core/libTouche_Core.a
  ${CMAKE_BINARY_DIR}/Settings/libTouche_Configuration.a
)

set(Touche_common_includes ${QT_INCLUDES}
  ${CMAKE_SOURCE_DIR}/Core ${CMAKE_BINARY_DIR}/Touche_Core
  ${CMAKE_SOURCE_DIR}/Settings ${CMAKE_BINARY_DIR}/Settings)


add_definitions(${QT_DEFINITIONS})
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
include(${QT_USE_FILE})

set(Touche_libs ${QT_LIBRARIES} ${Touche_static_libraries} ${X11_LIBRARIES} ${QJSON_LIBRARIES} ${X11_XTest_LIB} )


add_subdirectory(Core)
add_subdirectory(Settings)

if(TOUCHE_QT)
  add_subdirectory(qt)
else(TOUCHE_QT)
  add_subdirectory(kde)
endif(TOUCHE_QT)

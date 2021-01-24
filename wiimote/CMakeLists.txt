set(WiimoteBridge_SRCS
  angles.cpp
  cwiidconnectionworker.cpp
  timeutils.cpp
  wiimote.cpp
  wiimotemanager.cpp
  wiimote_messages.cpp
)
#set(CMAKE_CXX_FLAGS "${KDE_CXX_FLAGS} -noansi ")

string(REPLACE "-ansi" "" CMAKE_CXX_FLAGS ${KDE_CXX_FLAGS})

qt5_wrap_cpp(WiimoteBridge_mocs
  cwiidconnectionworker.h
  wiimote.h
  wiimotemanager.h
)

set(WiimoteBridge_includes ${Touche_common_includes} ${CWIID_INCLUDE_DIRS} )

include_directories(${WiimoteBridge_includes})
add_library(WiimoteBridge STATIC ${WiimoteBridge_SRCS} ${WiimoteBridge_mocs} )


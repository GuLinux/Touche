set(WiimoteLib_SRCS
  angles.cpp
  cwiidconnectionworker.cpp
  timeutils.cpp
  wiimote.cpp
  wiimotemanager.cpp
  wiimote_messages.cpp
)
#set(CMAKE_CXX_FLAGS "${KDE_CXX_FLAGS} -noansi ")

string(REPLACE "-ansi" "" CMAKE_CXX_FLAGS ${KDE_CXX_FLAGS})

qt4_wrap_cpp(WiimoteLib_mocs
  cwiidconnectionworker.h
  wiimote.h
  wiimotemanager.h
)

set(WiimoteLib_includes ${Touche_common_includes} ${CWIID_INCLUDE_DIRS} )

include_directories(${WiimoteLib_includes}) 
add_library(WiimoteLib STATIC ${WiimoteLib_SRCS} ${WiimoteLib_mocs} )


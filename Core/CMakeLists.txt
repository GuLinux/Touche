set(Touche_Core_SRCS
    backend/config/keyboarddatabase.cpp
    backend/config/databaseentry.cpp
    backend/config/bindingsconfig.cpp
    backend/hiddev/HiddevDevices.cpp
    backend/hiddev/hiddev.cpp
    backend/hiddev/hid_inputevent.cpp
    backend/updatekeysymbolmapping.cpp
    domain/dumpkeys.cpp
    domain/translatekeyevents.cpp
    domain/runcommandbinding.cpp
    domain/tokeybinding.cpp
    domain/deviceinfo.cpp
    domain/configevent.cpp
    domain/donothingbinding.cpp
    domain/nomoreeventsinputevent.cpp
    domain/Device.cpp
    domain/DevicesList.cpp
    touchecore.cpp
)

qt4_wrap_cpp(Touche_Core_mocs
  backend/hiddev/HiddevDevices.h
  backend/hiddev/hid_inputevent.h
  backend/hiddev/hiddev.h
  backend/config/databaseentry.h
  backend/config/bindingsconfig.h
  backend/config/keyboarddatabase.h
  backend/updatekeysymbolmapping.h
  domain/runcommandbinding.h
  domain/dumpkeys.h
  domain/nomoreeventsinputevent.h
  domain/deviceinfo.h
  domain/donothingbinding.h
  domain/configevent.h
  domain/translatekeyevents.h
  domain/tokeybinding.h
  domain/Device.h
  domain/DevicesList.h
  touchecore.h
)

set(Touche_Core_includes ${Touche_common_includes} )

include_directories(${Touche_Core_includes}) 
add_library(Touche_Core STATIC ${Touche_Core_SRCS} ${Touche_Core_mocs} )



set(Touche_Core_SRCS backend/hiddev/hiddev.cpp
    domain/dumpkeys.cpp
    domain/translatekeyevents.cpp
    domain/runcommandbinding.cpp
    domain/tokeybinding.cpp
    backend/hiddev/finddevices.cpp
    domain/deviceinfo.cpp
    backend/config/keyboarddatabase.cpp
    backend/config/databaseentry.cpp
    domain/inputevent.cpp
    domain/inputregister.cpp
    domain/configevent.cpp
    backend/config/bindingsconfig.cpp
    domain/donothingbinding.cpp
    touchecore.cpp
    domain/nomoreeventsinputevent.cpp
    backend/updatekeysymbolmapping.cpp)

qt4_wrap_cpp(Touche_Core_mocs
  backend/hiddev/finddevices.h
  backend/hiddev/hiddev.h
  backend/config/databaseentry.h
  backend/config/bindingsconfig.h
  backend/config/keyboarddatabase.h
  backend/updatekeysymbolmapping.h
  domain/runcommandbinding.h
  domain/dumpkeys.h
  domain/nomoreeventsinputevent.h
  domain/deviceinfo.h
  domain/inputevent.h
  domain/inputregister.h
  domain/donothingbinding.h
  domain/configevent.h
  domain/translatekeyevents.h
  domain/tokeybinding.h
  touchecore.h
)

set(Touche_Core_includes ${Touche_common_includes} )

include_directories(${Touche_Core_includes}) 
add_library(Touche_Core STATIC ${Touche_Core_SRCS} ${Touche_Core_mocs} )


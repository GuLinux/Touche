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

set(Touche_Core_includes ${Touche_common_includes} )

include_directories(${Touche_Core_includes}) 
add_library(Touche_Core STATIC ${Touche_Core_SRCS} )
target_link_libraries(Touche_Core Qt5::Widgets Qt5::X11Extras)


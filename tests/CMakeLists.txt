add_executable(HidDeviceTest hiddevicetest.cpp)
target_link_libraries(HidDeviceTest Touche_Core ${X11_LIBRARIES} ${X11_XTest_LIB} Qt5::Widgets )
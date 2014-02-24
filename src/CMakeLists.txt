SET(CMAKE_BUILD_TYPE Debug)
add_executable(udp2can 
    udp2can.c can.c udp.c
    can_driver.h can.h udp.h)

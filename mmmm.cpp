cmake_minimum_required(VERSION 3.12)
project(RPi_Project)

include_directories(/usr/include)
link_directories(/usr/lib/aarch64-linux-gnu)

find_library(GPIOD_LIB gpiod)
find_library(GPIODCXX_LIB gpiodcxx)

if (GPIOD_LIB AND GPIODCXX_LIB)
    message(STATUS "Found gpiod libraries: ${GPIOD_LIB}, ${GPIODCXX_LIB}")
    add_executable(sensor_project 
        src/main.cpp 
        src/Delay.cpp 
        src/DHT11.cpp 
        src/Key.cpp
    )
    target_link_libraries(sensor_project 
        PRIVATE 
        ${GPIOD_LIB}    
        ${GPIODCXX_LIB} 
    )
else()
    message(FATAL_ERROR "gpiod libraries not found!")
endif()
  

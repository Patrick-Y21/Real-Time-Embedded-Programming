cmake_minimum_required(VERSION 3.12)
project(Smart_Curtain_System)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(/usr/include)
link_directories(/usr/lib/aarch64-linux-gnu)

# Find required libraries
find_library(GPIOD_LIB gpiod)
find_library(GPIODCXX_LIB gpiodcxx)
find_package(Threads REQUIRED)

if (GPIOD_LIB AND GPIODCXX_LIB)
    message(STATUS "Found gpiod libraries: ${GPIOD_LIB}, ${GPIODCXX_LIB}")
    
    # Create executable
    add_executable(smart_curtain_system 
        main.cpp 
        Delay.cpp 
        DHT11.cpp 
        Key.cpp
        SystemController.cpp
    )
    
    # Link libraries
    target_link_libraries(smart_curtain_system 
        PRIVATE 
        ${GPIOD_LIB}    
        ${GPIODCXX_LIB}
        Threads::Threads
    )
    
    # Compiler flags
    target_compile_options(smart_curtain_system PRIVATE
        -Wall -Wextra -O2 -g
    )
    
else()
    message(FATAL_ERROR "gpiod libraries not found!")
endif()

# Create a test executable
option(BUILD_TESTS "Build test programs" ON)
if(BUILD_TESTS)
    add_executable(test_comprehensive
        test.cpp
        Delay.cpp
        DHT11.cpp
        Key.cpp
        SystemController.cpp
    )
    
    target_link_libraries(test_comprehensive
        PRIVATE
        ${GPIOD_LIB}
        ${GPIODCXX_LIB}
        Threads::Threads
    )
endif()

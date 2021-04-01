cmake_minimum_required(VERSION 3.5)

message(STATUS "Building deviceDetectors")
project (device-detectors)

add_library (device-detectors STATIC
    USBDetector.cpp
)

target_include_directories (device-detectors
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/deviceDetectors
        ${CMAKE_SOURCE_DIR}/src/utils
)

target_link_libraries (device-detectors
    PRIVATE
    udev
)

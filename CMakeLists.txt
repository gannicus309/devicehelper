cmake_minimum_required(VERSION 3.5)

project (devicehelper)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release")
endif()

find_package (
    PkgConfig
    REQUIRED
)
find_package (
    Boost
    COMPONENTS
        filesystem
        regex
    REQUIRED
)

pkg_check_modules (LIBUSB REQUIRED libusb-1.0)

add_subdirectory(src/deviceDetectors)
add_subdirectory(src/processor)
add_subdirectory(src)
cmake_minimum_required(VERSION 3.5)

project (devicehelper)

# C++ compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_compile_options(-Wextra)
# add_compile_options(-Wpedantic)
add_compile_options(-Wfloat-equal)
add_compile_options(-Werror=return-type)
option(LOGUSBMONITOR "Logs from USB monitor worker" OFF)
option(LOGLIBUSBHELPER "Logs from libusb for debugging" OFF)
option(LOGDEVLIST "Logs from devicelist and eventing for debugging" OFF)

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
pkg_check_modules (GIOMM REQUIRED giomm-2.4)

if(LOGUSBMONITOR)
    add_compile_definitions(LOGUSBMONITOR=1)
endif()
if(LOGLIBUSBHELPER)
    add_compile_definitions(LOGLIBUSBHELPER=1)
endif()
if(LOGDEVLIST)
    add_compile_definitions(LOGDEVICELIST=1)
    add_compile_definitions(LOGDEVICEEVENTLOOP=1)
endif()

add_subdirectory(src/deviceDetectors)
add_subdirectory(src/processor)
add_subdirectory(src)
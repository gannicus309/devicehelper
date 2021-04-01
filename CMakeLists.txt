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

add_subdirectory(src/deviceDetectors)
add_subdirectory(src/processor)
add_subdirectory(src)
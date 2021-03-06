message(STATUS "Building processor")


add_library (processor STATIC
    run.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/libusb_wrapper.cpp
    DevicehelperServer.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/DeviceEventLoop.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/DeviceInfoList.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/AAPConsts.cpp
)

target_include_directories (processor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/utils
        ${CMAKE_SOURCE_DIR}/src/deviceDetectors
        ${LIBUSB_INCLUDE_DIRS}
        ${GIOMM_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries (processor
    PRIVATE
        pthread
        device-detectors
        ${GIOMM_LIBRARIES}
        ${LIBUSB_LIBRARIES}
)
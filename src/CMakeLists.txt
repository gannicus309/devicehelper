
message(STATUS "Building devicehelper")

add_executable (devicehelper
    main.cpp
)

include_directories (devicehelper
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/deviceDetectors
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/processor
        ${CMAKE_SOURCE_DIR}/src/utils
)

add_dependencies(devicehelper processor device-detectors)

target_link_libraries (devicehelper
    PRIVATE
        processor
        device-detectors
)

install (
    TARGETS devicehelper
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
)

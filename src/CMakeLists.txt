
message(STATUS "Building devicehelper")

add_executable (devicehelper
    main.cpp
)

include_directories (devicehelper
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

install (
    TARGETS devicehelper
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
)

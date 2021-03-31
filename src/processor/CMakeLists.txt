message(STATUS "Building processor")


add_library (processor STATIC
    run.cpp
)

target_include_directories (processor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/utils
)

add_library(processor STATIC
    processor.h
)

target_include_directories(processor INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

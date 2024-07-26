add_library(memory STATIC
    memory.h
)

target_include_directories(memory INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

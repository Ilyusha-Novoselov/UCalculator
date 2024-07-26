add_library(history STATIC
    history.h
)

target_include_directories(history INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

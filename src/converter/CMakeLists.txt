add_library(converter STATIC
    converter_10_p.cpp
    converter_10_p.h
    converter_p_10.cpp
    converter_p_10.h
)

target_include_directories(converter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

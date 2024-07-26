add_library(numbers STATIC
    cnumber.h
    fnumber.h
    pnumber.h
    TANumber.h
)

target_include_directories(numbers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

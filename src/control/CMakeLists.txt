add_library(control STATIC
    AControl.cpp
    AControl.h
    ccontrol.cpp
    ccontrol.h
    fcontrol.cpp
    fcontrol.h
    pcontrol.cpp
    pcontrol.h
)

target_include_directories(control INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

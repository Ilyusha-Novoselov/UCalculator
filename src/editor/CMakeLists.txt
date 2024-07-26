add_library(editor STATIC
    AEditor.cpp
    AEditor.h
    ceditor.cpp
    ceditor.h
    feditor.cpp
    feditor.h
    peditor.cpp
    peditor.h
)

target_link_libraries(editor PRIVATE converter)
target_include_directories(editor INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

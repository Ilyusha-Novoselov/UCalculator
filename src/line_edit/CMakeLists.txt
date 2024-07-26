add_library(line_edit STATIC
    input_line_edit.h
)

target_link_libraries(line_edit PRIVATE Qt6::Core Qt6::Widgets)
target_include_directories(line_edit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

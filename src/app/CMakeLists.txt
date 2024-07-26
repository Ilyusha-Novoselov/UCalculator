add_executable(UCalculator
    mainwindow.ui
    mainwindow.cpp
    mainwindow.h
    main.cpp
)

target_link_libraries(UCalculator PRIVATE Qt6::Widgets)
target_link_libraries(UCalculator PRIVATE converter)
target_link_libraries(UCalculator PRIVATE control)
target_link_libraries(UCalculator PRIVATE line_edit)
target_link_libraries(UCalculator PRIVATE editor)
target_link_libraries(UCalculator PRIVATE history)
target_link_libraries(UCalculator PRIVATE memory)
target_link_libraries(UCalculator PRIVATE numbers)
target_link_libraries(UCalculator PRIVATE processor)

set_target_properties(UCalculator PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
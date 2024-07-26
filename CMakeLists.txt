cmake_minimum_required(VERSION 3.28)
project(UCalculator LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if (MSVC)
        add_compile_options(/Zc:__cplusplus)
endif()

add_subdirectory(src/converter)
add_subdirectory(src/numbers)

add_subdirectory(src/control)
add_subdirectory(src/editor)
add_subdirectory(src/line_edit)
add_subdirectory(src/history)
add_subdirectory(src/memory)
add_subdirectory(src/processor)

add_subdirectory(src/app)
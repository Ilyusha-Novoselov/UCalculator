cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(UCalculator LANGUAGES CXX)
elseif(UNIX)
    project(UCalculator)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(SOURCE_DIRS
    "App"
    "Control"
    "Converter"
    "Editor"
    "Forms"
    "History"
    "Memory"
    "Number"
    "Processor"
)

set(FORMS_DIR "${CMAKE_SOURCE_DIR}/Forms")

include_directories(${SOURCE_DIRS})

set(ALL_FILES)

foreach(SOURCE_DIR ${SOURCE_DIRS})
    file(GLOB_RECURSE FILES
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/*.h"
        "${SOURCE_DIR}/*.ui"
    )
    list(APPEND ALL_FILES ${FILES})
endforeach()

#=================== SETUP EXECTUABLE ====================#
# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

if (WIN32) 
    add_executable(UCalculator WIN32 ${ALL_FILES})
elseif(UNIX)
    add_executable(UCalculator ${ALL_FILES})
endif()

target_include_directories(UCalculator PRIVATE ${SOURCE_DIRS})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(UCalculator Qt6::Widgets)

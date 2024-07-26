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

if(NOT DEFINED ENV{QTDIR})
    message(FATAL_ERROR "Environment variable QTDIR is not set. Please set it to the Qt6 installation path.")
else()
    set(CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

if (MSVC)
        add_compile_options(/Zc:__cplusplus)
endif()

find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/form")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

#=================== SETUP EXECTUABLE ====================#
# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32) 
    add_executable(UCalculator WIN32 ${SOURCES})
elseif(UNIX)
    add_executable(UCalculator ${SOURCES})
endif()

# Add the target includes for MY_PROJECT 
target_include_directories(UCalculator PRIVATE ${FORMS_DIR})
target_include_directories(UCalculator PRIVATE ${INCLUDE_DIR})
target_include_directories(UCalculator PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(UCalculator Qt6::Widgets)
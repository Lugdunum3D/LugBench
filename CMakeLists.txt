cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# use macros
include(${CMAKE_SOURCE_DIR}/cmake/Macros.cmake)

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

if(ANDROID)
    populate_android_infos()
endif()

# project name
project(lugbench)

# define the executable
set(EXECUTABLE_NAME "lugbench")

# use config
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# find vulkan
find_package(Vulkan REQUIRED)
include_directories(${VULKAN_INCLUDE_DIR})

# find fmt
find_package(Fmt)

if (NOT FMT_INCLUDE_DIR)
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/fmt")
        message(FATAL_ERROR "Can't find fmt, call `git submodule update --recursive`")
    endif()

    set(FMT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/fmt/include)
    message(STATUS "Found Fmt: ${FMT_INCLUDE_DIR}")
endif()

include_directories(${FMT_INCLUDE_DIR})

# find json (it's not a well known library, so just find it in thirdparty)
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/json")
    message(FATAL_ERROR "Can't find json, call `git submodule update --recursive`")
endif()

set(JSON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/json/include)
message(STATUS "Found Json: ${JSON_INCLUDE_DIR}")

include_directories(${JSON_INCLUDE_DIR})

# find Lugdunum
find_package(LUG REQUIRED core graphics system window math)
include_directories(${LUG_INCLUDE_DIR})

set(SRC
    src/Application.cpp
    src/main.cpp
)

set(INC
    include/Application.hpp
)

# List of shaders in the directory ${LUG_RESOURCES_DIR}/shader
set(SHADERS
    shader.vert
    shader-directional.frag
    shader-point.frag
    shader-spot.frag
)

# List of resources in the directory ${LUG_RESOURCES_DIR}
set(LUG_RESOURCES
    # Nothing for the moment
)

# List of resources in the directory ${CMAKE_SOURCE_DIR}/resources
set(OTHER_RESOURCES
    # Nothing for the moment
)

# create target
if(LUG_OS_ANDROID)
    add_library(${EXECUTABLE_NAME} SHARED ${SRC} ${INC})

    set(ANDROID_PROJECT_PATH ${CMAKE_SOURCE_DIR}/android/${EXECUTABLE_NAME})
    set(ANDROID_PROJECT_ASSETS ${ANDROID_PROJECT_PATH}/src/main/assets)
    set(ANDROID_PROJECT_SHADERS ${ANDROID_PROJECT_PATH}/src/main/shaders)
else()
    add_executable(${EXECUTABLE_NAME} ${SRC} ${INC})
endif()

include_directories(include)
lug_add_compile_options(${EXECUTABLE_NAME})

target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})

# copy / build shaders
add_shaders(${EXECUTABLE_NAME} ${SHADERS})

# copy lugdunum resources
add_resources(${EXECUTABLE_NAME} lug_resources ${LUG_RESOURCES_DIR} ${LUG_RESOURCES})

# copy resources
add_resources(${EXECUTABLE_NAME} sample_resources "${CMAKE_SOURCE_DIR}/resources" ${OTHER_RESOURCES})

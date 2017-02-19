cmake_minimum_required(VERSION 3.1)

# determine the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(lugbench)

# define the executable
set(EXECUTABLE_NAME "lugbench")

if (NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/json/src"
    OR NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ext/lugdunum/src")
  message(FATAL_ERROR "call \"git submodule update --init --recursive\"")
endif()

# include json.hpp (header-only lib)
include_directories(thirdparty/json/src/)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# find Lugdunum and link with it
find_package(LUG COMPONENTS core graphics system window math)
if(NOT LUG_FOUND)
  message(STATUS "Lugdunum was not found. Building it instead")


  # build lugdunum libraries
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/lugdunum" "ext_build/lugdunum")
  # set the same variabel name as findLug
  set(LUG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/lugdunum/include)
  # path to genereated libs
  link_directories(${CMAKE_CURRENT_BINARY_DIR}/ext_build/lugdunum/lib)

  set(LUG_LIBRARIES "")
  list(APPEND LUG_LIBRARIES lug-window)
  list(APPEND LUG_LIBRARIES lug-system)
endif()
  

include_directories(${LUG_INCLUDE_DIR})


if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        list(APPEND CMAKE_CXX_FLAGS "/W4")
    endif()

    list(APPEND CMAKE_CXX_FLAGS "/WX")
else()
    list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++14")
endif()

string(REPLACE ";" " " "CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS}")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_library(${EXECUTABLE_NAME}
                SHARED
                include/Application.hpp src/Application.cpp src/main.cpp
    )

    target_link_libraries(${EXECUTABLE_NAME}
                          log
                          android
                          ${LUG_LIBRARIES}
    )
else()
    add_executable(${EXECUTABLE_NAME} include/Application.hpp src/Application.cpp src/main.cpp)
    target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
endif()

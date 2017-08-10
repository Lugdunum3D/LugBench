cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# use macros
include(${CMAKE_SOURCE_DIR}/cmake/Macros.cmake)

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# set the path of thirdparty
lug_set_option(LUGBENCH_THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty" STRING "Choose the path for the thirdparty directory")

if(ANDROID)
    populate_android_infos()
endif()

# project name
project(lugbench)

# define the executable
set(EXECUTABLE_NAME "lugbench")

# use config
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# find Vulkan
find_package(Vulkan)
if (NOT VULKAN_INCLUDE_DIR)
    if (NOT EXISTS "${LUGBENCH_THIRDPARTY_DIR}/vulkan")
        message(FATAL_ERROR "Can't find vulkan in the thirdparty directory")
    endif()

    set(VULKAN_INCLUDE_DIR ${LUGBENCH_THIRDPARTY_DIR}/vulkan/include)
    message(STATUS "Found Vulkan: ${VULKAN_INCLUDE_DIR}")
endif()

include_directories(${VULKAN_INCLUDE_DIR})

# find fmt
find_package(Fmt)

if (NOT FMT_INCLUDE_DIR)
    if (NOT EXISTS "${LUGBENCH_THIRDPARTY_DIR}/fmt")
        message(FATAL_ERROR "Can't find fmt in the thirdparty directory")
    endif()

    set(FMT_INCLUDE_DIR ${LUGBENCH_THIRDPARTY_DIR}/fmt/include)
    message(STATUS "Found Fmt: ${FMT_INCLUDE_DIR}")
endif()

include_directories(${FMT_INCLUDE_DIR})

if (NOT EXISTS "${LUGBENCH_THIRDPARTY_DIR}/json")
    message(FATAL_ERROR "Can't find json in the thirdparty directory")
endif()

set(JSON_INCLUDE_DIR ${LUGBENCH_THIRDPARTY_DIR}/json/include)
message(STATUS "Found Json: ${JSON_INCLUDE_DIR}")

include_directories(${JSON_INCLUDE_DIR})

# we don't use RestClientCpp on android
if(NOT LUG_OS_ANDROID)
    find_package(RestClientCpp)
    if (NOT RESTCLIENTCPP_FOUND)
        if (NOT EXISTS "${LUGBENCH_THIRDPARTY_DIR}/restclient")
            message(FATAL_ERROR "Can't restclient fmt in the thirdparty directory")
        endif()

        set(RESTCLIENTCPP_ROOT ${LUGBENCH_THIRDPARTY_DIR}/restclient)
        find_package(RestClientCpp REQUIRED)
    endif()

    include_directories(${RESTCLIENTCPP_INCLUDE_DIR})

    # restclient-cpp depends of curl
    find_package(CURL)
    if (NOT CURL_FOUND)
        if (NOT EXISTS "${LUGBENCH_THIRDPARTY_DIR}/curl")
            message(FATAL_ERROR "Can't find curl in the thirdparty directory")
        endif()

        set(CMAKE_PREFIX_PATH ${LUGBENCH_THIRDPARTY_DIR}/curl)
        find_package(CURL REQUIRED)
        message(STATUS "Found Curl: ${CURL_INCLUDE_DIR}")
    endif()

    include_directories(${CURL_INCLUDE_DIR})
    list(APPEND RESTCLIENTCPP_LIBRARIES ${CURL_LIBRARIES})
endif()

# find Lugdunum
find_package(LUG REQUIRED core graphics system window math)
include_directories(${LUG_INCLUDE_DIR})

set(SRC
    src/Application.cpp
    src/GPUInfoProvider.cpp
    src/GUI.cpp
    src/main.cpp
    src/MenuState.cpp
    src/BenchmarkingState.cpp
    src/LugNetwork.cpp
)

set(INC
    include/Application.hpp
    include/GPUInfoProvider.hpp
    include/GUI.hpp
    include/AState.hpp
    include/MenuState.hpp
    include/BenchmarkingState.hpp
    include/LugNetwork.hpp
)

# find imgui
if (NOT EXISTS "${LUGBENCH_THIRDPARTY_DIR}/imgui")
    message(FATAL_ERROR "Can't find imgui, call `git submodule update --recursive`")
endif()

set(IMGUI_INCLUDE_DIR ${LUGBENCH_THIRDPARTY_DIR}/imgui/include)
message(STATUS "Found Imgui: ${IMGUI_INCLUDE_DIR}")

set(IMGUI_SOURCE_DIR ${LUGBENCH_THIRDPARTY_DIR}/imgui/src)
message(STATUS "Found Imgui: ${IMGUI_SOURCE_DIR}")

include_directories(${IMGUI_INCLUDE_DIR})

# List of shaders in the directory ${LUG_RESOURCES_DIR}/shader
set(SHADERS
    gui.vert
    gui.frag
)
# List of resources in the directory ${LUG_RESOURCES_DIR}
set(LUG_RESOURCES
    # Nothing for the moment
    shaders/forward/shader.vert
    shaders/forward/shader.frag
    models/DamagedHelmet/DamagedHelmet.bin
    models/DamagedHelmet/DamagedHelmet.gltf
    models/DamagedHelmet/textures/Default_albedo.jpg
    models/DamagedHelmet/textures/Default_AO.jpg
    models/DamagedHelmet/textures/Default_emissive.jpg
    models/DamagedHelmet/textures/Default_metallic_roughness.jpg
    models/DamagedHelmet/textures/Default_normal.jpg
)

# List of resources in the directory ${CMAKE_SOURCE_DIR}/resources
set(OTHER_RESOURCES
    models/Duck/Duck0.bin
    models/Duck/Duck.gltf
    models/Duck/DuckCM.png
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

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES} Threads::Threads)

if(RESTCLIENTCPP_LIBRARIES)
    target_link_libraries(${EXECUTABLE_NAME} ${RESTCLIENTCPP_LIBRARIES})
endif()

# copy / build shaders
add_shaders(${EXECUTABLE_NAME} ${SHADERS})

# copy lugdunum resources
add_resources(${EXECUTABLE_NAME} lug_resources ${LUG_RESOURCES_DIR} ${LUG_RESOURCES})

# copy resources
add_resources(${EXECUTABLE_NAME} sample_resources "${CMAKE_SOURCE_DIR}/resources" ${OTHER_RESOURCES})

# add api uri
set(DEFAULT_LUGBENCH_API_URI "http://lugbench-api-dev.herokuapp.com")

lug_set_option(LUGBENCH_API_URI ${DEFAULT_LUGBENCH_API_URI} STRING "URI of the Lugbench API")
add_definitions(/DLUGBENCH_API_URI="${LUGBENCH_API_URI}")

# add api VERSION
lug_set_option(LUGBENCH_API_VERSION "1" STRING "Version of the Lugbench API")
add_definitions(/DLUGBENCH_API_VERSION="${LUGBENCH_API_VERSION}")

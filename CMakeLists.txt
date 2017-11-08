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

# set the path of thirdparty
lug_set_option(LUG_THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty" STRING "Choose the path for the thirdparty directory")

# download thirdparty dir if not specified
lug_download_thirdparty()

# use config
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# find Vulkan
find_package(Vulkan)
if (NOT VULKAN_INCLUDE_DIR)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/vulkan")
        message(FATAL_ERROR "Can't find vulkan in the thirdparty directory")
    endif()

    set(VULKAN_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/vulkan/include)
    message(STATUS "Found Vulkan: ${VULKAN_INCLUDE_DIR}")
endif()

include_directories(${VULKAN_INCLUDE_DIR})

# find fmt
find_package(Fmt)

if (NOT FMT_INCLUDE_DIR)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/fmt")
        message(FATAL_ERROR "Can't find fmt in the thirdparty directory")
    endif()

    set(FMT_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/fmt/include)
    message(STATUS "Found Fmt: ${FMT_INCLUDE_DIR}")
endif()

include_directories(${FMT_INCLUDE_DIR})

if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/json")
    message(FATAL_ERROR "Can't find json in the thirdparty directory")
endif()

set(JSON_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/json/include)
message(STATUS "Found Json: ${JSON_INCLUDE_DIR}")

include_directories(${JSON_INCLUDE_DIR})

# find Lugdunum

find_package(LUG QUIET COMPONENTS core graphics system window math)
if (NOT LUG_FOUND)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/lugdunum")
        message(FATAL_ERROR "Can't find lugdunum in the thirdparty directory")
    endif()

    set(LUG_ROOT ${LUG_THIRDPARTY_DIR}/lugdunum)
    find_package(LUG REQUIRED COMPONENTS core graphics system window math)
endif()

include_directories(${LUG_INCLUDE_DIR})


set(SRC
    src/Application.cpp
    src/GPUInfoProvider.cpp
    src/GUI.cpp
    src/main.cpp
    src/BenchmarkingState.cpp
    src/BenchmarksState.cpp
    src/ModelsState.cpp
    src/ModelViewer.cpp
    src/ResultsState.cpp
    src/InfoState.cpp
    src/ContactState.cpp
)

set(INC
    include/Application.hpp
    include/GPUInfoProvider.hpp
    include/GUI.hpp
    include/AState.hpp
    include/BenchmarkingState.hpp
    include/BenchmarksState.hpp
    include/ModelsState.hpp
    include/ModelViewer.hpp
    include/ResultsState.hpp
    include/InfoState.hpp
    include/ContactState.hpp
)

# find imgui
if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/imgui")
    message(FATAL_ERROR "Can't find imgui in the thirdparty directory")
endif()

set(IMGUI_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/imgui/include)
message(STATUS "Found Imgui: ${IMGUI_INCLUDE_DIR}")

include_directories(${IMGUI_INCLUDE_DIR})

# IconFontAwesome
include_directories(ext/)

# List of shaders in the directory ${LUG_RESOURCES_DIR}/shader
set(SHADERS
    gui.vert
    gui.frag
    skybox.frag
    skybox.vert
)

# List of resources in the directory ${LUG_RESOURCES_DIR}
set(LUG_RESOURCES
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
    fonts/fontawesome-webfont.ttf
    fonts/Roboto-Bold.ttf
    fonts/Roboto-Light.ttf
    textures/epitech_logo.png
    textures/epitech_color_logo.png
    textures/gltf_logo.png
    textures/vulkan_logo.png
    textures/lugbench_logo.png
    textures/lugdunum_logo.png
    textures/skybox/back.jpg
    textures/skybox/bottom.jpg
    textures/skybox/front.jpg
    textures/skybox/left.jpg
    textures/skybox/right.jpg
    textures/skybox/top.jpg
    textures/licence_logo.png
    textures/lugbench-short.png
    textures/lugdunum-short.png
    textures/info_device_logo.png
    licences/apache-licence.txt
    licences/fmt-licence.txt
    licences/freetype-licence.txt
    licences/googlemock-licence.txt
    licences/imgui-licence.txt
    licences/json-licence.txt
    licences/lugdunum3d-licence.txt
)

# create target
if(LUG_OS_ANDROID)
    add_library(${EXECUTABLE_NAME} SHARED ${SRC} ${INC})
    set(EXT_LIBRARIES android)

    set(ANDROID_PROJECT_PATH ${CMAKE_SOURCE_DIR}/android/${EXECUTABLE_NAME})
    set(ANDROID_PROJECT_ASSETS ${ANDROID_PROJECT_PATH}/src/main/assets)
    set(ANDROID_PROJECT_SHADERS ${ANDROID_PROJECT_PATH}/src/main/assets/shaders)
else()
    add_executable(${EXECUTABLE_NAME} ${SRC} ${INC})
endif()

include_directories(include)
lug_add_compile_options(${EXECUTABLE_NAME})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES} Threads::Threads ${EXT_LIBRARIES})

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

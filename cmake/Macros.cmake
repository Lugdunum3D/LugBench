include(CMakeParseArguments)

set(LUG_SHADERS_DIR "shaders")
set(PROJECT_SHADERS_ROOT "shaders")

# define a macro that helps defining an option
macro(lug_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

macro(populate_android_infos)
    set(ANDROID_ABI arm64-v8a)
    message(STATUS "ANDROID_ABI: ${ANDROID_ABI}")

    set(ANDROID_TOOLCHAIN clang)
    message(STATUS "ANDROID_TOOLCHAIN: ${ANDROID_TOOLCHAIN}")

    lug_set_option(ANDROID_PLATFORM android-24 STRING "Target specific android version target (default: android-24)")
    if(NOT ANDROID_PLATFORM MATCHES android-2[4-9])
        message(FATAL_ERROR "Invalid android version target: ${ANDROID_PLATFORM}")
    endif()
    message(STATUS "ANDROID_PLATFORM: ${ANDROID_PLATFORM}")

    lug_set_option(ANDROID_STL c++_shared STRING "Choose which Android STL to use (default: c++_shared)")

    if(NOT ANDROID_STL STREQUAL c++_static AND NOT ANDROID_STL STREQUAL c++_shared)
        message(FATAL_ERROR "Invalid STL: ${ANDROID_STL}")
    endif()

    set(LIB_SUFFIX "/${ANDROID_ABI}")
endmacro()

macro(lug_add_compile_options target)
    # c++14 on android
    if(LUG_OS_ANDROID)
        target_compile_options(${target} PUBLIC -std=c++14)
    endif()

    # use warnings and errors
    if(LUG_COMPILER_MSVC)
        target_compile_options(${target} PUBLIC /W4 /WX)
    elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
        target_compile_options(${target} PUBLIC -Wall -Wextra -Werror)
    endif()

    # add parallel build for MSVC
    if(LUG_COMPILER_MSVC)
        target_compile_options(${target} PUBLIC /MP)
    endif()
endmacro()

# shaders
macro(add_shader shader)
    # Select where to copy the resource
    if(LUG_OS_ANDROID)
        set(new_path ${ANDROID_PROJECT_SHADERS}/${shader}.spv)
    else()
        set(new_path ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_SHADERS_ROOT}/${shader}.spv)
    endif()

    set(old_path ${LUG_RESOURCES_DIR}/${LUG_SHADERS_DIR}/${shader})
    get_filename_component(new_path_directory ${new_path} DIRECTORY)

    if(LUG_OS_ANDROID)
        execute_process(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE_NAME)
        execute_process(COMMAND uname -s COMMAND tr -d '\n' OUTPUT_VARIABLE OS_NAME)

        add_custom_command(
            OUTPUT ${new_path}
            DEPENDS ${old_path}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${new_path_directory}
            COMMAND ${ANDROID_NDK}/shader-tools/${OS_NAME}-${ARCHITECTURE_NAME}/glslc ${old_path} -o ${new_path}
        )
    else()
        # Compile the shader
        add_custom_command(
            OUTPUT ${new_path}
            DEPENDS ${old_path}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${new_path_directory}
            COMMAND glslangValidator -V ${old_path} -o ${new_path}

            COMMENT "Compiling shader ${old_path} to ${new_path}"
        )
    endif()

    list(APPEND SHADERS_DEPENDS ${new_path})
endmacro()

macro(add_shaders target)
    lug_set_option(BUILD_SHADERS TRUE BOOL "Compile shaders")

    if(BUILD_SHADERS)
        foreach(shader ${ARGN})
            add_shader(${shader})
        endforeach(shader)
    endif()

    add_custom_target(shaders DEPENDS ${SHADERS_DEPENDS})
    add_dependencies(${target} shaders)
endmacro()

# resources
macro(add_resource target_resources directory resource)
    # Select where to copy the resource
    if(LUG_OS_ANDROID)
        set(new_path ${ANDROID_PROJECT_ASSETS}/${resource})
    else()
        set(new_path ${CMAKE_CURRENT_BINARY_DIR}/${resource})
    endif()

    set(old_path ${directory}/${resource})
    get_filename_component(new_path_directory ${new_path} DIRECTORY)

    # Custom command to create the directory and copy the resource
    add_custom_command(
        OUTPUT ${new_path}
        DEPENDS ${old_path}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${new_path_directory}
        COMMAND ${CMAKE_COMMAND} -E copy ${old_path} ${new_path}

        COMMENT "Copying ${old_path} to ${new_path}"
    )

    list(APPEND ${target_resources}_DEPENDS ${new_path})
endmacro()

macro(add_resources target target_resources directory)
    # Copy all resources
    foreach(resource ${ARGN})
        add_resource(${target_resources} ${directory} ${resource})
    endforeach(resource)

    # Link new resources files to the target
    add_custom_target(${target_resources} DEPENDS ${${target_resources}_DEPENDS})
    add_dependencies(${target} ${target_resources})
endmacro()

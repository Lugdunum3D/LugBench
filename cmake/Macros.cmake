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
	string(TOLOWER ${OS_NAME} OS_NAME)
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

function(lug_download_thirdparty)
    lug_set_option(LUG_THIRDPARTY_URL "https://thirdparty-dl.lugbench.eu" STRING "Choose the server from which to download the thirdparty directory")
    lug_set_option(LUG_ACCEPT_DL OFF BOOL "Choose whether to accept or not the download of the thirdparty directory")

    string(TOLOWER "${CMAKE_SYSTEM_NAME}" THIRDPARTY_PLATFORM)

    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/.git")
        message(FATAL_ERROR "Can't find the .git directory")
    endif()

    find_package(Git)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -n 1 --pretty=format:%h -- thirdparty.yml
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            OUTPUT_VARIABLE "THIRDPARTY_SHORT_HASH"
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Thirdparty hash: ${THIRDPARTY_LATEST_HASH}")
    else()
        message(FATAL_ERROR "Can't find the latest revision of `thirdparty.yml`")
    endif()

    if (EXISTS "${LUG_THIRDPARTY_DIR}")
        if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/version")
            message(STATUS "Thirdparty dir exists and version is UNKNOWN, using as-is")
            return()
        endif()

        file(STRINGS "${LUG_THIRDPARTY_DIR}/version" LUG_THIRDPARTY_VERSION)
        if (${LUG_THIRDPARTY_VERSION} STREQUAL "${THIRDPARTY_PLATFORM}_${THIRDPARTY_SHORT_HASH}")
            message(STATUS "Thirdparty dir exists and version is OK")
            return()
        endif()
        message(STATUS "Thirdparty dir exists but is outdated (${LUG_THIRDPARTY_VERSION}, needs ${THIRDPARTY_PLATFORM}_${THIRDPARTY_SHORT_HASH}), updating...")
        file(REMOVE_RECURSE "${LUG_THIRDPARTY_DIR}")
    endif()

    if (NOT "${LUG_ACCEPT_DL}")
        message(
            FATAL_ERROR
            "\n"
            "NOT FOUND: ${LUG_THIRDPARTY_DIR}\n"
            "---\n"
            "The thirdparty directory was NOT found, "
            "but we can download it for your architecture. "
            "To protect your privacy the download is not automatic.\n"
            "To proceed and accept the download, re-run cmake with -DLUG_ACCEPT_DL=ON\n"
            "---\n"
            "You can also compile it from source: see https://github.com/Lugdunum3D/ThirdParty-Builder\n"
            "---\n"
            "Thanks,\n"
            "The Lugdunum3D Team.\n"
            "\n"
        )
    endif()

    set(SHOULD_DOWNLOAD ON)
    set(DL_FILE "${CMAKE_BINARY_DIR}/thirdparty-${THIRDPARTY_SHORT_HASH}.zip")
    set(DL_URL "${LUG_THIRDPARTY_URL}/${THIRDPARTY_SHORT_HASH}/${THIRDPARTY_PLATFORM}.zip")

    # Checksums
    set(DL_FILE_EXPECTED_MD5 "${CMAKE_BINARY_DIR}/thirdparty-${THIRDPARTY_SHORT_HASH}.md5")
    set(DL_URL_EXPECTED_MD5 "${LUG_THIRDPARTY_URL}/${THIRDPARTY_SHORT_HASH}/${THIRDPARTY_PLATFORM}.md5")

    file(DOWNLOAD "${DL_URL_EXPECTED_MD5}" "${DL_FILE_EXPECTED_MD5}")
    file(STRINGS "${DL_FILE_EXPECTED_MD5}" DL_EXPECTED_MD5)
    message(STATUS "Expected MD5 will be '${DL_EXPECTED_MD5}'")

    # Check MD5 if file exists
    if (EXISTS "${DL_FILE}")
        file(MD5 "${DL_FILE}" DL_FILE_MD5)
        if (DL_FILE_MD5 STREQUAL DL_EXPECTED_MD5)
            message(STATUS "Already exists and checks valid: ${DL_FILE}")
            set(SHOULD_DOWNLOAD OFF)
        else()
            message(STATUS "Already exists but not valid: ${DL_FILE}")
        endif()
    endif()

    if(SHOULD_DOWNLOAD)
        message(STATUS "Downloading ${DL_URL}")
        file(
            DOWNLOAD
            "${DL_URL}" "${DL_FILE}"
            EXPECTED_MD5 "${DL_EXPECTED_MD5}"
        )
    endif()

    # Extract zip
    message(STATUS "Extracting...")
    file(MAKE_DIRECTORY "${LUG_THIRDPARTY_DIR}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xfz "${DL_FILE}"
        WORKING_DIRECTORY "${LUG_THIRDPARTY_DIR}"
        RESULT_VARIABLE rv
    )
    if (NOT rv EQUAL 0)
        file(REMOVE_RECURSE "${LUG_THIRDPARTY_DIR}")
        message(
            FATAL_ERROR
            "Extract of '${DL_FILE}' failed"
            "Try to remove it to download it again"
        )
    endif()

    file(WRITE "${LUG_THIRDPARTY_DIR}/version" "${THIRDPARTY_PLATFORM}_${THIRDPARTY_SHORT_HASH}")
    message(STATUS "Done!")
endfunction()

function(lug_download_models file_name)
    set(DL_URL "${LUG_THIRDPARTY_URL}/models/${file_name}.zip")
    set(DL_FILE "${CMAKE_SOURCE_DIR}/resources/models/${file_name}.zip")
    set(UNZIP_DEST "${CMAKE_BINARY_DIR}/models/${file_name}")

    if (NOT EXISTS ${DL_FILE})
        message(STATUS "Downloading ${DL_URL}")
        file(
            DOWNLOAD
            "${DL_URL}" "${DL_FILE}"
        )
    endif()

    # Extract zip
    message(STATUS "Extracting...")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/models/")
#    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/models/")
    file(MAKE_DIRECTORY ${UNZIP_DEST})
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xfz "${DL_FILE}"
        WORKING_DIRECTORY ${UNZIP_DEST}
        RESULT_VARIABLE rv
    )
    if (NOT rv EQUAL 0)
        file(REMOVE ${UNZIP_DEST})
        message(
            FATAL_ERROR
            "Extract of '${DL_FILE}' failed"
            "Try to remove it to download it again"
        )
    endif()

    message(STATUS "Done!")
endfunction()

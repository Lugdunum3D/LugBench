# Find Vulkan
#
# VULKAN_INCLUDE_DIR
# VULKAN_LIBRARY
# VULKAN_FOUND

if (WIN32)

    find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
        "$ENV{VULKAN_SDK}/Include"
        "$ENV{VK_SDK_PATH}/Include")
    if (CMAKE_CL_64)
        find_library(VULKAN_LIBRARY NAMES vulkan-1 HINTS
            "$ENV{VULKAN_SDK}/lib"
            "$ENV{VK_SDK_PATH}/lib")
        find_library(VULKAN_STATIC_LIBRARY NAMES vkstatic.1 HINTS
            "$ENV{VULKAN_SDK}/lib"
            "$ENV{VK_SDK_PATH}/lib")
    else()
        find_library(VULKAN_LIBRARY NAMES vulkan-1 HINTS
            "$ENV{VULKAN_SDK}/lib32"
            "$ENV{VK_SDK_PATH}/lib32")
    endif()
elseif (APPLE)
    find_library(VULKAN_LIBRARY MoltenVK)
    if (VULKAN_LIBRARY)
        set(VULKAN_STATIC_LIBRARY ${VULKAN_LIBRARY})
        find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
            "${VULKAN_LIBRARY}/Headers")
    endif()
else()
    find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
        "$ENV{VULKAN_SDK}/include")
    find_library(VULKAN_LIBRARY NAMES vulkan HINTS
        "$ENV{VULKAN_SDK}/lib")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vulkan DEFAULT_MSG VULKAN_LIBRARY VULKAN_INCLUDE_DIR)

mark_as_advanced(VULKAN_INCLUDE_DIR VULKAN_LIBRARY VULKAN_STATIC_LIBRARY)


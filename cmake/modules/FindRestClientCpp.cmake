# This script locates the restclient_cpp library
# ------------------------------------
#

# define the list of search paths for headers and libraries
set(FIND_RESTCLIENTCPP_PATHS
    ${RESTCLIENTCPP_ROOT}
    $ENV{RESTCLIENTCPP_ROOT}
    /usr/local
    /usr
    /opt/local
    /opt
)

find_path(RESTCLIENTCPP_ROOT_DIR include/restclient-cpp/restclient.h
               PATHS ${FIND_RESTCLIENTCPP_PATHS}
               CMAKE_FIND_ROOT_PATH_BOTH
)

message(STATUS ${RESTCLIENTCPP_ROOT_DIR})

find_path(RESTCLIENTCPP_INCLUDE_DIR
    NAMES restclient-cpp/restclient.h
    PATHS $ENV{RESTCLIENTCPP_ROOT} ${RESTCLIENTCPP_ROOT} ${RESTCLIENTCPP_ROOT_DIR}
    PATH_SUFFIXES include
    CMAKE_FIND_ROOT_PATH_BOTH
)

find_library(RESTCLIENTCPP_LIBRARY_DEBUG
    NAMES restclient-cpp-d
    PATHS $ENV{RESTCLIENTCPP_ROOT} ${RESTCLIENTCPP_ROOT} ${RESTCLIENTCPP_ROOT_DIR}
    PATH_SUFFIXES lib
    CMAKE_FIND_ROOT_PATH_BOTH
)
message(STATUS ${RESTCLIENTCPP_LIBRARY_DEBUG})

find_library(RESTCLIENTCPP_LIBRARY_RELEASE
    NAMES restclient-cpp
    PATHS $ENV{RESTCLIENTCPP_ROOT} ${RESTCLIENTCPP_ROOT} ${RESTCLIENTCPP_ROOT_DIR}
    PATH_SUFFIXES lib
    CMAKE_FIND_ROOT_PATH_BOTH
)
message(STATUS ${RESTCLIENTCPP_LIBRARY_RELEASE})

# if both are found, set RESTCLIENTCPP_LIBRARIES to contain both
if (RESTCLIENTCPP_LIBRARY_DEBUG AND RESTCLIENTCPP_LIBRARY_RELEASE)
    set(RESTCLIENTCPP_LIBRARIES debug ${RESTCLIENTCPP_LIBRARY_DEBUG}
                        optimized ${RESTCLIENTCPP_LIBRARY_RELEASE})
endif()

# if only one debug/release variant is found, set the other to be equal to the found one
if (RESTCLIENTCPP_LIBRARY_DEBUG AND NOT RESTCLIENTCPP_LIBRARY_RELEASE)
    set(RESTCLIENTCPP_LIBRARY_RELEASE ${RESTCLIENTCPP_LIBRARY_DEBUG})
    set(RESTCLIENTCPP_LIBRARIES ${RESTCLIENTCPP_LIBRARY_DEBUG})
endif()

if (NOT RESTCLIENTCPP_LIBRARY_DEBUG AND RESTCLIENTCPP_LIBRARY_RELEASE)
    set(RESTCLIENTCPP_LIBRARY_DEBUG ${RESTCLIENTCPP_LIBRARY_RELEASE})
    set(RESTCLIENTCPP_LIBRARIES ${RESTCLIENTCPP_LIBRARY_RELEASE})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RestClientCpp DEFAULT_MSG RESTCLIENTCPP_LIBRARIES RESTCLIENTCPP_INCLUDE_DIR)

if (RESTCLIENTCPP_FOUND)
    message(STATUS "Found restclient_cpp headers in ${RESTCLIENTCPP_INCLUDE_DIR}")
    message(STATUS "Found restclient_cpp libraries in ${RESTCLIENTCPP_LIBRARIES}")
endif()

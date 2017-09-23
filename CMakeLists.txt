cmake_minimum_required(VERSION 3.8)
project(multiGet)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads REQUIRED)
add_definitions (-DNN_HAVE_OSX)

FIND_PACKAGE(CURL)
IF(CURL_FOUND)
    INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
    SET(REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} ${CURL_LIBRARIES} )
ELSE(CURL_FOUND)
    MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

add_subdirectory (src)


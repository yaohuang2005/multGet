cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Configuration.cpp
        Writer.cpp
        FileChunkDownloader.cpp
        MultiGet.cpp
        Manager.cpp
        MGException.h
)

add_executable(multiGet ${SOURCE_FILES})

target_link_libraries(multiGet ${CURL_LIBRARIES})
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17 )
project(src)

SET(TARGET_NAME "src")

file(GLOB_RECURSE TARGET_HEADERS "*.cpp")
file(GLOB_RECURSE TARGET_SOURCES "*.hpp")

add_library(${TARGET_NAME} STATIC ${TARGET_HEADERS} ${TARGET_SOURCES})

#=====================================
#外部环境依赖
#=====================================

#OpenCV
find_package(OpenCV REQUIRED)
target_link_directories(${TARGET_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PUBLIC ${OpenCV_LIBS})


#Boost

find_package(Boost REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PUBLIC ${Boost_LIBRARY})

if(CMAKE_SYSTEM_NAME MATCHES  "Linux")
    find_package(Threads)
    target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_THREADS_LIBS_INIT})
endif()




cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

set(TARGET_NAME "test")

file(GLOB_RECURSE TARGET_SOURCE "*.cpp")
file(GLOB_RECURSE TARGET_HEADER "*.hpp")

add_executable(${TARGET_NAME} ${TARGET_SOURCE} ${TARGET_HEADER})

target_include_directories(${TARGET_NAME} PRIVATE "../")
target_link_libraries(${TARGET_NAME} PRIVATE src)

#thread
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(Threads)
    target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()
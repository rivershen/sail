cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(UseDoxygen OPTIONAL)

project (C4)

find_package( Threads REQUIRED )

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
        add_definitions(-std=c++11)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()    

include_directories(public dependencies/gtest/include dependencies/jsoncpp)
add_subdirectory(dependencies/gtest)
add_subdirectory(dependencies/jsoncpp)
add_subdirectory(unittests)
add_subdirectory(examples)

cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)

find_package(Boost REQUIRED)

if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found. Please install Boost and try again.")
endif()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(shell ${SOURCE_FILES})

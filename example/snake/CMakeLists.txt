cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(snake LANGUAGES CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Please create a /build directory")
endif()

if(NOT TARGET kronkworld_static)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/kronkworld)
endif()

file(GLOB_RECURSE SNAKE_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(SNAKE_SRCS)
    add_executable(snake ${SNAKE_SRCS})
    target_include_directories(snake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(snake PRIVATE kronkworld_static sfml-window sfml-graphics sfml-system)
    target_compile_options(snake PRIVATE -Wall -Wextra -O3)
else()
    message(WARNING "No test source files found in ${CMAKE_CURRENT_SOURCE_DIR}/src/")
endif()

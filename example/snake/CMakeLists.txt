cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(snake LANGUAGES CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Please create a /build directory")
endif()

# include(FetchContent)
# FetchContent_Declare(
#     kronklab
#     GIT_REPOSITORY https://github.com/kronkorp/kronklab
#     GIT_TAG        main
# )
# FetchContent_MakeAvailable(kronklab)

if(NOT TARGET kronkworld)
    add_library(kronkworld INTERFACE)

    target_include_directories(kronkworld INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
    )
endif()

file(GLOB_RECURSE SNAKE_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(SNAKE_SRCS)
    add_executable(snake ${SNAKE_SRCS})
    target_link_libraries(snake PRIVATE kronkworld kronklab)
    target_compile_options(snake PRIVATE -Wall -Wextra -O3)
else()
    message(WARNING "No test source files found in ${CMAKE_CURRENT_SOURCE_DIR}/src/")
endif()

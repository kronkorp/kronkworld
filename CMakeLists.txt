cmake_minimum_required(VERSION 3.15)

project(kronkworld LANGUAGES CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

include(FetchContent)
FetchContent_Declare(
    kronklab
    GIT_REPOSITORY https://github.com/kronkorp/kronklab
    GIT_TAG        main
)
FetchContent_MakeAvailable(kronklab)

add_library(kronkworld INTERFACE)

target_compile_features(kronkworld INTERFACE cxx_std_20)

target_include_directories(kronkworld INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

file(GLOB_RECURSE kronkworld_TEST_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/*.cpp"
)

if(kronkworld_TEST_SOURCES)
    add_executable(kronkworld_tests ${kronkworld_TEST_SOURCES})
    target_link_libraries(kronkworld_tests PRIVATE kronkworld kronklab)
    target_compile_options(kronkworld_tests PRIVATE -Wall -Wextra -O3)
else()
    message(WARNING "No test source files found in ${CMAKE_CURRENT_SOURCE_DIR}/tests/")
endif()

file(GLOB_RECURSE kronkworld_BENCH_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/benchmark/*.cpp"
)

if(kronkworld_BENCH_SOURCES)
    add_executable(kronkworld_bench ${kronkworld_BENCH_SOURCES})
    target_link_libraries(kronkworld_bench PRIVATE kronkworld kronklab)
    target_compile_options(kronkworld_bench PRIVATE -Wall -Wextra -O3)
else()
    message(WARNING "No test source files found in ${CMAKE_CURRENT_SOURCE_DIR}/tests/")
endif()

include(GNUInstallDirs)

install(TARGETS kronkworld
    EXPORT kronkworldTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronkworld
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

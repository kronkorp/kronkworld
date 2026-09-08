cmake_minimum_required(VERSION 3.15)

project(kronkworld LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

option(BUILD_TESTS "Build KronkWorld unit tests" OFF)

include(FetchContent)

if(BUILD_TESTS)
    FetchContent_Declare(
        kronklab
        GIT_REPOSITORY https://github.com/kronkorp/kronklab
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(kronklab)
endif()

FetchContent_Declare(
    kronkflow
    GIT_REPOSITORY https://github.com/kronkorp/kronkflow
    GIT_TAG        main
)
FetchContent_MakeAvailable(kronkflow)

option(KRONKWORLD_INSTALL "Install KronkWorld libraries and headers" OFF)

file(GLOB_RECURSE kronkworld_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(kronkworld_shared SHARED ${kronkworld_SOURCES})
add_library(kronkworld_static STATIC ${kronkworld_SOURCES})

add_library(kronkworld::shared ALIAS kronkworld_shared)
add_library(kronkworld::static ALIAS kronkworld_static)

set_target_properties(kronkworld_shared PROPERTIES EXPORT_NAME shared OUTPUT_NAME kronkworld)
set_target_properties(kronkworld_static PROPERTIES EXPORT_NAME static OUTPUT_NAME kronkworld)

target_compile_features(kronkworld_shared PUBLIC cxx_std_20)
target_compile_features(kronkworld_static PUBLIC cxx_std_20)

target_include_directories(kronkworld_shared PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(kronkworld_static PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(kronkworld_shared PUBLIC kronkflow)
target_link_libraries(kronkworld_static PUBLIC kronkflow)

target_sources(kronkworld_shared PRIVATE $<TARGET_OBJECTS:kronkflow-obj>)
target_sources(kronkworld_static PRIVATE $<TARGET_OBJECTS:kronkflow-obj>)

if(BUILD_TESTS)
    file(GLOB_RECURSE kronkworld_TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/*.cpp"
    )

    if(kronkworld_TEST_SOURCES)
        add_executable(kronkworld_tests ${kronkworld_TEST_SOURCES})
        target_link_libraries(kronkworld_tests PRIVATE kronkworld_shared kronklab)
        target_compile_options(kronkworld_tests PRIVATE -Wall -Wextra -O3)
    else()
        message(WARNING "No test source files found in ${CMAKE_CURRENT_SOURCE_DIR}/tests/")
    endif()
endif()

include(GNUInstallDirs)

if(KRONKWORLD_INSTALL)
    install(TARGETS kronkworld_shared kronkworld_static
        EXPORT kronkworldTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT kronkworldTargets
        FILE kronkworldTargets.cmake
        NAMESPACE kronkworld::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kronkworld
    )

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronkworld
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

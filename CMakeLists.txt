cmake_minimum_required(VERSION 3.15)

project(kronkworld LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

option(BUILD_TESTS "Build KronkWorld unit tests" OFF)

include(FetchContent)

# NOTE: Pinned: KronkWorld does not change when their main does
if(BUILD_TESTS)
    # NOTE: kronklab runs each test in a fork(), which Windows does not have
    if(WIN32)
        message(FATAL_ERROR "The tests need kronklab, which runs on POSIX only: configure with -DBUILD_TESTS=OFF")
    endif()

    FetchContent_Declare(
        kronklab
        GIT_REPOSITORY https://github.com/kronkorp/kronklab
        GIT_TAG        ed591c8d0172558ca72eecac4f55c2bb1344625a
    )
    FetchContent_MakeAvailable(kronklab)
endif()

FetchContent_Declare(
    kronkflow
    GIT_REPOSITORY https://github.com/kronkorp/kronkflow
    GIT_TAG        d1c604712de759e58a65fa22825a70cd4eae3871
)
# NOTE: kronkflow reads BUILD_TESTS too: its tests are not ours to build
set(KRONKWORLD_BUILD_TESTS ${BUILD_TESTS})
set(BUILD_TESTS OFF)
FetchContent_MakeAvailable(kronkflow)
set(BUILD_TESTS ${KRONKWORLD_BUILD_TESTS})

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

if(MSVC)
    # NOTE: The import library of the DLL is already kronkworld.lib
    set_target_properties(kronkworld_static PROPERTIES OUTPUT_NAME kronkworld_static)
endif()

# NOTE: On Windows, a DLL only gives what it exports: the API is not marked for
#       it, so it exports all
set_target_properties(kronkworld_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

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

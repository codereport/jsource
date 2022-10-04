cmake_minimum_required(VERSION 3.17)

get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (is_multi_config)
    set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING
            "Semicolon separated list of supported configuration types")
    mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
elseif (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_C_FLAGS)
    message(WARNING "No CMAKE_BUILD_TYPE is selected")
endif ()

project(j)
enable_language(C CXX ASM)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CTest)

if (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(BUILD_SHARED_LIBS ON)
endif ()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(DOCS "NO" CACHE STRING "Build documentation")

add_compile_options($<$<COMPILE_LANGUAGE:C>:-fno-strict-aliasing>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,CXX,GNU>:-fmax-errors=1>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,CXX,AppleClang,Clang>:-ferror-limit=1>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-fPIE>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-fPIC>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wfatal-errors>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-fdiagnostics-color=always>)

# Treat warnings as errors
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Werror>)

# Exclude warnings from errors
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-error=sequence-point>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-error=strict-overflow>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-error=logical-not-parentheses>)

# Enable warnings
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wall>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wextra>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wunused-macros>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wno-error=unused-macros>) # Some false positives / only cover current build configuration

# Exclude some warnings
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 8.0)
    # this warning was added in gcc 8 https://debarshiray.wordpress.com/2019/04/01/about-wextra-and-wcast-function-type/
    add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wno-cast-function-type>)
endif ()
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-misleading-indentation>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-address>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wno-unused-parameter>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wno-unused-value>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wno-unused-variable>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-unused-but-set-variable>)
add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wno-sign-compare>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-clobbered>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-empty-body>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-parentheses>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-pointer-sign>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-format-overflow>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-implicit-fallthrough>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-missing-field-initializers>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-shift-negative-value>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-maybe-uninitialized>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-uninitialized>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wno-type-limits>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-char-subscripts>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,AppleClang,Clang>:-Wno-string-plus-int>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-missing-braces>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-unknown-pragmas>)

add_subdirectory(jsrc)
add_subdirectory(base64)

# see https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html for method used to have cmake git submodule
find_package(Git REQUIRED)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
endif ()
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/An-Algorithm-Library/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif ()
add_subdirectory(An-Algorithm-Library EXCLUDE_FROM_ALL)
if (${DOCS} STREQUAL "YES")
    add_subdirectory(docs)
endif ()

if (BUILD_SHARED_LIBS)
    set_target_properties(j-base64 PROPERTIES
            POSITION_INDEPENDENT_CODE ON)
    add_subdirectory(test)
endif ()
target_link_libraries(j PRIVATE j-base64)

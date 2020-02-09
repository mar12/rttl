cmake_minimum_required(VERSION 2.8)

project(rttl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(".")

# UnitTest++ library
find_package(UnitTest++ REQUIRED)
include_directories(SYSTEM ${UTPP_INCLUDE_DIRS})

set(RTTL_SOURCES "rttl/string.h"
                 "rttl/vector.h")

# Unit Tests
add_executable(TestString "test/test_string.cpp" ${RTTL_SOURCES})
target_link_libraries(TestString UnitTest++)
target_link_options(TestString INTERFACE --coverage)

add_executable(TestVector "test/test_vector.cpp" "test/element.h" "test/input_iterator.h" ${RTTL_SOURCES})
target_link_libraries(TestVector UnitTest++)
target_link_options(TestVector INTERFACE --coverage)



if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    # using Clang
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wall -Wextra -Werror -pedantic -Wformat=2 -Wformat-signedness -Wsuggest-final-types -Wsuggest-final-methods -Warray-bounds=2 -Wfloat-equal -Wcast-qual -Wcast-align -Wuseless-cast -Wconversion -Wparentheses -Wlogical-op -O0 -g")
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
    include(CodeCoverage)
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(NAME coverage EXECUTABLE TestVector BASE_DIRECTORY "${PROJECT_SOURCE_DIR}" EXCLUDE "test/*")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
    # using Intel C++
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # using Visual Studio C++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

message(${CMAKE_CXX_FLAGS})


enable_testing()
add_test(NAME TestString COMMAND TestString)
add_test(NAME TestVector COMMAND TestVector)

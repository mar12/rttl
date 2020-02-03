cmake_minimum_required(VERSION 2.8)
project(rttl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wall -Wextra -Werror -pedantic -Wformat=2 -Wformat-signedness -Wsuggest-final-types -Wsuggest-final-methods -Warray-bounds=2 -Wfloat-equal -Wcast-qual -Wcast-align -Wuseless-cast -Wconversion -Wparentheses -Wlogical-op -O0 -g --coverage")

find_package(UnitTest++ REQUIRED)
include_directories(${UTPP_INCLUDE_DIRS})

add_executable(UnitTests "test_string.cpp" "string.h")
target_link_libraries(UnitTests UnitTest++)
target_link_options(UnitTests INTERFACE --coverage)

enable_testing()
add_test(NAME UnitTests COMMAND UnitTests)

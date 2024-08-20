cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)

project(sdk-ul-mu-pair)

set(test_dir ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# set(EXECUTABLE_OUTPUT_PATH ${test_dir})
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${lib_dir})
aux_source_directory(${lib_dir} lib_list)
set(a pair)
add_library(${a} ${lib_list})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")

else()
    enable_testing()
    find_package(GTest REQUIRED)

    set(exe test.out)
    add_executable(${exe} ${test_dir}/case.cpp)
    target_link_libraries(${exe} ${a} ${GTEST_BOTH_LIBRARIES})

    gtest_discover_tests(${exe})
endif()
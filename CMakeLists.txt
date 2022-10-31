cmake_minimum_required(VERSION 3.21)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

# NOTE: huffman functional must be implemented as library and linked to the executables
# NOTE: tool target name must be huffman-tool

include_directories(huflib)

set(huflib
        huflib/archiver.cpp
        huflib/buffered_reader.cpp
        huflib/buffered_writer.cpp
        huflib/decoder.cpp
        huflib/encoder.cpp
        huflib/huffman_tree.cpp
        huflib/huffman_exception.cpp
        huflib/IO_exception.cpp
        )

add_executable(tests
        ${huflib}
        unit-tests/tests.cpp)

add_executable(huffman-tool
        ${huflib}
        huffman-tool.cpp
        huflib/IO_exception.h huflib/huffman_exception.cpp huflib/huffman_exception.h huflib/IO_exception.cpp)
# don't forget to use same flags at your library 
if (NOT MSVC)
  target_compile_options(tests PRIVATE -Wall -Wno-sign-compare -pedantic)
endif()

option(USE_SANITIZERS "Enable to build with undefined,leak and address sanitizers" OFF)
if (USE_SANITIZERS)
  target_compile_options(tests PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
  target_link_options(tests PUBLIC -fsanitize=address,undefined,leak)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(tests PUBLIC -stdlib=libc++)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  target_compile_options(tests PUBLIC -D_GLIBCXX_DEBUG)
endif()

target_link_libraries(tests GTest::gtest GTest::gtest_main)

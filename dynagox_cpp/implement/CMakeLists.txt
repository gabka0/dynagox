cmake_minimum_required(VERSION 3.23)

project(DynaGox)
set(CMAKE_CXX_STANDARD 23)

enable_testing()
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(ClangFormat)
include(fmt)

add_subdirectory(convert)
add_subdirectory(experiments)
add_subdirectory(multi_hash_map)
add_subdirectory(tests)
add_subdirectory(utils)

clang_format(clang-format-convert ${CMAKE_SOURCE_DIR}/convert)
clang_format(clang-format-experiments ${CMAKE_SOURCE_DIR}/experiments)
clang_format(clang-format-multi-hash-map ${CMAKE_SOURCE_DIR}/multi_hash_map)
clang_format(clang-format-tests ${CMAKE_SOURCE_DIR}/tests)
clang_format(clang-format-utils ${CMAKE_SOURCE_DIR}/utils)
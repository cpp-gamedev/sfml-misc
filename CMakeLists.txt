cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_EXTENSIONS OFF)

project(sfml-misc)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.?pp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

if(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<NOT:$<CONFIG:Debug>>:/WX>)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra $<$<NOT:$<CONFIG:Debug>>:-Werror>)
endif()


cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF)

project(world-clock)

add_subdirectory(ext/sfml)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  message(STATUS "Exporting compile_commands.json")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# git
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  message(STATUS "Updating git submodules...")
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    RESULT_VARIABLE UPDATE_SUBMODULES_RESULT
  )
  if(NOT UPDATE_SUBMODULES_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update failed!")
  endif()
endif()

# exe
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.?pp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /MP $<$<NOT:$<CONFIG:Debug>>:/WX>)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra $<$<NOT:$<CONFIG:Debug>>:-Werror>)
endif()

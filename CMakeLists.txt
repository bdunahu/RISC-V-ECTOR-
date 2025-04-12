cmake_minimum_required(VERSION 3.5)
project(risc_vector)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --remote --recursive
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  RESULT_VARIABLE GIT_SUBMOD_RESULT)
  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
  endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ram/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -lstdc++ -g -O0)
add_compile_options(-Wextra -Wpedantic)

set(RAM ram)

# cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# don't build RAM's tests
set(RAM_TESTS OF CACHE BOOL "" FORCE)

# add submodules
add_subdirectory(${RAM})
add_subdirectory(gui)

# gather source files
file(GLOB_RECURSE SRCS "src/*.cc")

# find QT components
find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()

# binary executable
add_library(${PROJECT_NAME}_lib ${SRCS})
target_include_directories(${PROJECT_NAME}_lib PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(${PROJECT_NAME}_lib ${RAM}_lib)

find_package(Catch2 REQUIRED)

#gather test files
file(GLOB_RECURSE TESTS "tests/*.cc")

# test executable
add_executable(test_rv ${SRCS} ${TESTS})
target_include_directories(test_rv PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(test_rv PRIVATE Catch2::Catch2WithMain PRIVATE ${RAM}_lib)

# test discovery
include(CTest)
include(Catch)
catch_discover_tests(test_rv)

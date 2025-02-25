cmake_minimum_required(VERSION 3.11)
project(risc_vector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(SRCDIR src)
set(PYTHON_VERSION 3.10)

find_package(Python3 COMPONENTS Development REQUIRED)

add_executable(${PROJECT_NAME} ${SRCDIR}/rv.cc)

target_link_libraries(${PROJECT_NAME} Python3::Python)

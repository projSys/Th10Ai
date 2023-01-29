cmake_minimum_required(VERSION 3.20)

project(Th10Ai)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)

add_subdirectory(Base)
add_subdirectory(Th10Base)
add_subdirectory(Th10Hook)
add_subdirectory(Th10Ai)

cmake_minimum_required(VERSION 3.20)

project(Th10Ai)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${ROOT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

#set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost REQUIRED COMPONENTS log filesystem)
#message(${Boost_LIBRARIES})

add_compile_definitions(_UNICODE
	UNICODE)

add_compile_options("/utf-8")

include_directories("${BOOST_DIR}")

link_directories("${BOOST_DIR}/stage/lib"
	"${BOOST_DIR}/lib32-msvc-14.3"
	"${LIBRARY_OUTPUT_PATH}")

add_subdirectory(Base)
add_subdirectory(Th10Base)
add_subdirectory(Th10Hook)
add_subdirectory(Th10Ai)

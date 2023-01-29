cmake_minimum_required(VERSION 3.20)

#set(LIBRARY_OUTPUT_PATH ../lib)
#set(EXECUTABLE_OUTPUT_PATH ../bin)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

#set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost REQUIRED COMPONENTS log filesystem)
#message(${Boost_LIBRARIES})

add_definitions(-D_UNICODE -DUNICODE)
#add_definitions(-DBOOST_USE_WINAPI_VERSION=0x0A00)
#add_compile_definitions(BOOST_USE_WINAPI_VERSION=0x0A00)

add_compile_options("/utf-8")

include_directories("${BOOST_DIR}"
	"${DETOURS_DIR}/include"
	"../Base/include"
	"../Th10Base/include"
	"./include")

aux_source_directory(./src/Th10Hook TH10HOOK_SRCS)

link_directories("${BOOST_DIR}/stage/lib"
	"${DETOURS_DIR}/lib.X86"
	"${LIBRARY_OUTPUT_PATH}")

link_libraries(dxguid.lib
	detours.lib
	Base.lib
	Th10Base.lib)

add_library(Th10Hook SHARED ${TH10HOOK_SRCS})

add_dependencies(Th10Hook Base Th10Base)

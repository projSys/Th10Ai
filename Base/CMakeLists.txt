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
	"./include")

aux_source_directory(./src/Base BASE_SRCS)
aux_source_directory(./src/Base/Exception BASE_SRCS)
aux_source_directory(./src/Base/Windows BASE_SRCS)

add_library(Base STATIC ${BASE_SRCS})

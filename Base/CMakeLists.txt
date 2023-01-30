cmake_minimum_required(VERSION 3.20)

include_directories("./include")

aux_source_directory(./src/Base BASE_SRCS)
aux_source_directory(./src/Base/Exception BASE_SRCS)
aux_source_directory(./src/Base/Windows BASE_SRCS)

add_library(Base STATIC ${BASE_SRCS})

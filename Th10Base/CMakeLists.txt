cmake_minimum_required(VERSION 3.20)

include_directories("../Base/include"
	"./include")

aux_source_directory(./src/Th10Base TH10BASE_SRCS)

add_library(Th10Base STATIC ${TH10BASE_SRCS})

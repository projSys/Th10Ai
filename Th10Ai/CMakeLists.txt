cmake_minimum_required(VERSION 3.20)

include_directories("../Base/include"
	"../Th10Base/include"
	"./include")

aux_source_directory(./src/Th10Ai TH10AI_SRCS)

link_libraries(Base.lib
	Th10Base.lib)

add_executable(Th10Ai ${TH10AI_SRCS})

add_dependencies(Th10Ai Base Th10Base)

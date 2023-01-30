cmake_minimum_required(VERSION 3.20)

include_directories("${DETOURS_DIR}/include"
	"../Base/include"
	"../Th10Base/include"
	"./include")

aux_source_directory(./src/Th10Hook TH10HOOK_SRCS)

link_directories("${DETOURS_DIR}/lib.X86")

link_libraries(dxguid.lib
	detours.lib
	Base.lib
	Th10Base.lib)

add_library(Th10Hook SHARED ${TH10HOOK_SRCS})

add_dependencies(Th10Hook Base Th10Base)

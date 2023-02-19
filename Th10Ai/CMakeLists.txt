cmake_minimum_required(VERSION 3.20)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS
	WXUSINGDLL)

include_directories("${WXWIDGETS_DIR}/include"
	"${WXWIDGETS_DIR}/include/msvc"
	"../Base/include"
	"../Th10Base/include"
	"./include")

aux_source_directory(./src/Th10Ai TH10AI_SRCS)

link_directories("${WXWIDGETS_DIR}/lib/vc_dll")

link_libraries(Base.lib
	Th10Base.lib)

add_executable(Th10Ai WIN32 ${TH10AI_SRCS})

add_dependencies(Th10Ai Base Th10Base)

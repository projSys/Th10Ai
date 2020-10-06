#pragma once

#include <Base/Common.h>

namespace base
{
#ifdef BASE_WINDOWS
	#ifdef BASE_EXPORTS
		#define DLL_API __declspec(dllexport)
	#else
		#define DLL_API __declspec(dllimport)
	#endif
#endif
}

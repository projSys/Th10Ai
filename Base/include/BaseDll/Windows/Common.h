#pragma once

#include "Base/Types.h"
#include "Base/String.h"

namespace base
{
	namespace win
	{
#ifdef BASE_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
	}
}

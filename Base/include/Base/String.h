#pragma once

#include "Base/Common.h"

#include <string>

namespace base
{
	class String
	{
	public:
		static bool IsEmpty(const char* str);
		static bool IsEmpty(const wchar_t* wstr);
	};
}

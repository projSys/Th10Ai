#pragma once

#include "Base/Common.h"

#include <string>

namespace base
{
	class String
	{
	public:
		static std::wstring Utf8ToWide(const char* ptr);
		static std::wstring Utf8ToWide(const std::string& str);
		static std::string WideToUtf8(const wchar_t* wptr);
		static std::string WideToUtf8(const std::wstring& wstr);
	};
}

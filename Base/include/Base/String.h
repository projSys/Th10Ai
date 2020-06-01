#pragma once

#include <string>

namespace base
{
	class String
	{
	public:
		static std::wstring Utf8ToWide(const char* str);
		static std::wstring Utf8ToWide(const std::string& str);
		static std::string WideToUtf8(const wchar_t* str);
		static std::string WideToUtf8(const std::wstring& str);
	};
}

#pragma once

#include <string>

namespace base
{
	class String
	{
	public:
		static wstring Utf8ToWide(const char* str);
		static wstring Utf8ToWide(const string& str);
		static string WideToUtf8(const wchar_t* str);
		static string WideToUtf8(const wstring& str);
	};
}

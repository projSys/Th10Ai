#include "Base/String.h"

#include <locale>
#include <codecvt>

namespace base
{
	// deprecated in C++17
	std::wstring String::Utf8ToWide(const char* ptr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(ptr);
	}

	std::wstring String::Utf8ToWide(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	std::string String::WideToUtf8(const wchar_t* wptr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(wptr);
	}

	std::string String::WideToUtf8(const std::wstring& wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}
}

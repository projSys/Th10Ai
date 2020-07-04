#include "Base/Common.h"
#include "Base/String.h"

#include <boost/locale.hpp>

namespace base
{
	using namespace boost::locale::conv;

	wstring String::Utf8ToWide(const char* str)
	{
		return utf_to_utf<wchar_t>(str);
	}

	wstring String::Utf8ToWide(const string& str)
	{
		return utf_to_utf<wchar_t>(str);
	}

	string String::WideToUtf8(const wchar_t* str)
	{
		return utf_to_utf<char>(str);
	}

	string String::WideToUtf8(const wstring& str)
	{
		return utf_to_utf<char>(str);
	}
}

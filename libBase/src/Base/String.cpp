#include "Base/String.h"

namespace base
{
	bool String::IsEmpty(const char* str)
	{
		return str == nullptr || str[0] == '\0';
	}

	bool String::IsEmpty(const wchar_t* wstr)
	{
		return wstr == nullptr || wstr[0] == L'\0';
	}
}

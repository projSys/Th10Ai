#pragma once

#include "Base/Windows/Common.h"

namespace base
{
	namespace win
	{
		class Apis
		{
		public:
			static constexpr DWORD BUFFER_SIZE = 1024;

			static std::wstring Utf8ToWide(const char* ptr);
			static std::wstring Utf8ToWide(const std::string& str);
			static std::string WideToUtf8(const wchar_t* wptr);
			static std::string WideToUtf8(const std::wstring& wstr);

			static std::string GetErrorDesc(DWORD errorId);

			static std::string GetModuleName(HMODULE module = nullptr);
			static std::string GetModuleDir(HMODULE module = nullptr);
		};
	}
}

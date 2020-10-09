#pragma once

#include "Base/Windows/Common.h"

#include "Base/String.h"

namespace base
{
	namespace win
	{
		class Apis
		{
		public:
			static constexpr DWORD BUFFER_SIZE = 1024;

			static std::wstring Utf8ToWide(const char* str);
			static std::wstring Utf8ToWide(const std::string& str);
			static std::string WideToUtf8(const wchar_t* wstr);
			static std::string WideToUtf8(const std::wstring& wstr);

			static std::wstring AnsiToWide(const char* str);
			static std::wstring AnsiToWide(const std::string& str);
			static std::string WideToAnsi(const wchar_t* wstr);
			static std::string WideToAnsi(const std::wstring& wstr);

			static std::string GetErrorDesc(DWORD errorCode);

			static std::string GetModulePath(HMODULE module = nullptr);
			static std::string GetModuleDir(HMODULE module = nullptr);
		};
	}
}

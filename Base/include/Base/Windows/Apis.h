#pragma once

#include "Base/Windows/Common.h"

#include <string>
#include <filesystem>

namespace base
{
	namespace win
	{
		namespace fs = std::filesystem;

		class Apis
		{
		public:
			static std::wstring MultiByteToWideChar(UINT codePage, const char* str, int strSize = -1);
			static std::string WideCharToMultiByte(UINT codePage, const wchar_t* wstr, int wstrSize = -1);

			static std::wstring Utf8ToWide(const char* str, int strSize = -1);
			static std::wstring Utf8ToWide(const std::string& str);
			static std::string WideToUtf8(const wchar_t* wstr, int wstrSize = -1);
			static std::string WideToUtf8(const std::wstring& wstr);

			static std::wstring AnsiToWide(const char* str, int strSize = -1);
			static std::wstring AnsiToWide(const std::string& str);
			static std::string WideToAnsi(const wchar_t* wstr, int wstrSize = -1);
			static std::string WideToAnsi(const std::wstring& wstr);

			static fs::path GetModulePath(HMODULE module = nullptr);
			static fs::path GetModuleDir(HMODULE module = nullptr);

		private:
			static constexpr DWORD BUFFER_SIZE = 2 * 1024;
		};
	}
}

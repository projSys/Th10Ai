#include "Base/Windows/Apis.h"

#include "Base/Windows/WindowsError.h"
#include "Base/Traits.h"

#pragma warning(disable: 6387)

namespace base
{
	namespace win
	{
		fs::path Apis::GetModulePath(HMODULE module)
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				Throw(WindowsError(GetLastError()));
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				Throw(WindowsError(ERROR_INSUFFICIENT_BUFFER));
			return fs::path(buffer);
		}

		fs::path Apis::GetModuleDir(HMODULE module)
		{
			return GetModulePath(module).parent_path();
		}

		// 有BUG
		std::wstring Apis::MultiByteToWideChar(UINT codePage, const char* str, int strSize)
		{
			std::wstring wstr;

			int wstrSize = ::MultiByteToWideChar(codePage, 0, str, strSize, nullptr, 0);
			if (wstrSize == 0)
				Throw(WindowsError(GetLastError()));

			wstr.resize(wstrSize + 10);
			int ret = ::MultiByteToWideChar(codePage, 0, str, strSize, &wstr[0], wstrSize + 10);
			if (ret == 0)
				Throw(WindowsError(GetLastError()));
			return wstr;
		}

		// 有BUG
		std::string Apis::WideCharToMultiByte(UINT codePage, const wchar_t* wstr, int wstrSize)
		{
			std::string str;

			int strSize = ::WideCharToMultiByte(codePage, 0, wstr, wstrSize, nullptr, 0,
				nullptr, nullptr);
			if (strSize == 0)
				Throw(WindowsError(GetLastError()));

			str.resize(strSize);
			int ret = ::WideCharToMultiByte(codePage, 0, wstr, wstrSize, &str[0], strSize,
				nullptr, nullptr);
			if (ret == 0)
				Throw(WindowsError(GetLastError()));
			return str;
		}

		std::wstring Apis::Utf8ToWide(const char* str, int strSize)
		{
			if (is_empty_string(str))
				return std::wstring();
			return MultiByteToWideChar(CP_UTF8, str, strSize);
		}

		std::wstring Apis::Utf8ToWide(const std::string& str)
		{
			if (str.empty())
				return std::wstring();
			return MultiByteToWideChar(CP_UTF8, str.c_str(), str.size());
		}

		std::string Apis::WideToUtf8(const wchar_t* wstr, int wstrSize)
		{
			if (is_empty_string(wstr))
				return std::string();
			return WideCharToMultiByte(CP_UTF8, wstr, wstrSize);
		}

		std::string Apis::WideToUtf8(const std::wstring& wstr)
		{
			if (wstr.empty())
				return std::string();
			return WideCharToMultiByte(CP_UTF8, wstr.c_str(), wstr.size());
		}

		std::wstring Apis::AnsiToWide(const char* str, int strSize)
		{
			if (is_empty_string(str))
				return std::wstring();
			return MultiByteToWideChar(CP_ACP, str, strSize);
		}

		std::wstring Apis::AnsiToWide(const std::string& str)
		{
			if (str.empty())
				return std::wstring();
			return MultiByteToWideChar(CP_ACP, str.c_str(), str.size());
		}

		std::string Apis::WideToAnsi(const wchar_t* wstr, int wstrSize)
		{
			if (is_empty_string(wstr))
				return std::string();
			return WideCharToMultiByte(CP_ACP, wstr, wstrSize);
		}

		std::string Apis::WideToAnsi(const std::wstring& wstr)
		{
			if (wstr.empty())
				return std::string();
			return WideCharToMultiByte(CP_ACP, wstr.c_str(), wstr.size());
		}
	}
}

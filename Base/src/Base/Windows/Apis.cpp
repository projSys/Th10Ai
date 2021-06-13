#include "Base/Windows/Apis.h"

#include <boost/filesystem.hpp>

#include "Base/Traits.h"
#include "Base/Windows/SystemError.h"

namespace base
{
	namespace win
	{
		std::wstring Apis::MultiByteToWideChar(UINT codePage, const char* str, int strSize)
		{
			int wstrSize = ::MultiByteToWideChar(codePage, 0, str, strSize, nullptr, 0);
			if (wstrSize == 0)
				BASE_THROW(SystemError, GetLastError());

			std::wstring wstr(wstrSize, L'\0');
			int ret = ::MultiByteToWideChar(codePage, 0, str, strSize, &wstr[0], wstrSize);
			if (ret == 0)
				BASE_THROW(SystemError, GetLastError());
			return wstr;
		}

		std::string Apis::WideCharToMultiByte(UINT codePage, const wchar_t* wstr, int wstrSize)
		{
			int strSize = ::WideCharToMultiByte(codePage, 0, wstr, wstrSize, nullptr, 0,
				nullptr, nullptr);
			if (strSize == 0)
				BASE_THROW(SystemError, GetLastError());

			std::string str(strSize, '\0');
			int ret = ::WideCharToMultiByte(codePage, 0, wstr, wstrSize, &str[0], strSize,
				nullptr, nullptr);
			if (ret == 0)
				BASE_THROW(SystemError, GetLastError());
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

		fs::path Apis::GetModulePath(HMODULE module)
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				BASE_THROW(SystemError, GetLastError());
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				BASE_THROW(SystemError, GetLastError());

			return fs::path(buffer);
		}

		fs::path Apis::GetModuleDir(HMODULE module)
		{
			return GetModulePath(module).parent_path();
		}
	}
}

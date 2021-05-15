#include "Base/Windows/Apis.h"

#include <system_error>

#include "Base/Exception.h"
#include "Base/TypeTraits.h"

namespace base
{
	namespace win
	{
		std::wstring Apis::MultiByteToWideChar(UINT codePage, LPCCH str, int strSize)
		{
			int wstrSize = ::MultiByteToWideChar(codePage, 0, str, strSize, nullptr, 0);
			if (wstrSize == 0)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));

			std::wstring wstr(wstrSize, L'\0');
			int ret = ::MultiByteToWideChar(codePage, 0, str, strSize, &wstr[0], wstrSize);
			if (ret == 0)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return wstr;
		}

		std::string Apis::WideCharToMultiByte(UINT codePage, LPCWCH wstr, int wstrSize)
		{
			int strSize = ::WideCharToMultiByte(codePage, 0, wstr, wstrSize, nullptr, 0,
				nullptr, nullptr);
			if (strSize == 0)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));

			std::string str(strSize, '\0');
			int ret = ::WideCharToMultiByte(codePage, 0, wstr, wstrSize, &str[0], strSize,
				nullptr, nullptr);
			if (ret == 0)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return str;
		}

		std::wstring Apis::Utf8ToWide(const char* str)
		{
			if (is_empty_string(str))
				return std::wstring();

			return MultiByteToWideChar(CP_UTF8, str, -1);
		}

		std::wstring Apis::Utf8ToWide(const std::string& str)
		{
			if (str.empty())
				return std::wstring();

			return MultiByteToWideChar(CP_UTF8, str.c_str(), str.size());
		}

		std::string Apis::WideToUtf8(const wchar_t* wstr)
		{
			if (is_empty_string(wstr))
				return std::string();

			return WideCharToMultiByte(CP_UTF8, wstr, -1);
		}

		std::string Apis::WideToUtf8(const std::wstring& wstr)
		{
			if (wstr.empty())
				return std::string();

			return WideCharToMultiByte(CP_UTF8, wstr.c_str(), wstr.size());
		}

		std::wstring Apis::AnsiToWide(const char* str)
		{
			if (is_empty_string(str))
				return std::wstring();

			return MultiByteToWideChar(CP_ACP, str, -1);
		}

		std::wstring Apis::AnsiToWide(const std::string& str)
		{
			if (str.empty())
				return std::wstring();

			return MultiByteToWideChar(CP_ACP, str.c_str(), str.size());
		}

		std::string Apis::WideToAnsi(const wchar_t* wstr)
		{
			if (is_empty_string(wstr))
				return std::string();

			return WideCharToMultiByte(CP_ACP, wstr, -1);
		}

		std::string Apis::WideToAnsi(const std::wstring& wstr)
		{
			if (wstr.empty())
				return std::string();

			return WideCharToMultiByte(CP_ACP, wstr.c_str(), wstr.size());
		}
	}
}

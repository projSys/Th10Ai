#include "Base/Windows/Apis.h"

#include "Base/Windows/WindowsError.h"

namespace base
{
	namespace win
	{
		std::string Apis::GetErrorDesc(DWORD errorCode)
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, errorCode, 0, buffer, BUFFER_SIZE - 1, nullptr);
			//if (ret == 0)
			//	BASE_THROW(WindowsError());

			return WideToUtf8(buffer);
		}

		std::wstring MultiByteToWideChar(UINT codePage, LPCCH str, int strSize)
		{
			int wstrSize = ::MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS,
				str, strSize, nullptr, 0);
			if (wstrSize == 0)
				BASE_THROW(WindowsError());

			std::wstring wstr(wstrSize, L'\0');
			int ret = ::MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS,
				str, strSize, &wstr[0], wstrSize);
			if (ret == 0)
				BASE_THROW(WindowsError());
			return wstr;
		}

		std::string WideCharToMultiByte(UINT codePage, LPCWCH wstr, int wstrSize)
		{
			int strSize = ::WideCharToMultiByte(codePage, WC_ERR_INVALID_CHARS,
				wstr, wstrSize, nullptr, 0, nullptr, nullptr);
			if (strSize == 0)
				BASE_THROW(WindowsError());

			std::string str(strSize, '\0');
			int ret = ::WideCharToMultiByte(codePage, WC_ERR_INVALID_CHARS,
				wstr, wstrSize, &str[0], strSize, nullptr, nullptr);
			if (ret == 0)
				BASE_THROW(WindowsError());
			return str;
		}

		std::wstring Apis::Utf8ToWide(const char* str)
		{
			if (String::IsEmpty(str))
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
			if (String::IsEmpty(wstr))
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
			if (String::IsEmpty(str))
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
			if (String::IsEmpty(wstr))
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

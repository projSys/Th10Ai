#include "Base/Windows/Apis.h"

#include <boost/filesystem.hpp>

#include "Base/Windows/WindowsError.h"

namespace base
{
	namespace win
	{
		namespace filesystem = boost::filesystem;

		std::wstring Apis::Utf8ToWide(const char* ptr)
		{
			if (ptr == nullptr || ptr[0] == '\0')
				return std::wstring();

			int wstrSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
				ptr, -1, nullptr, 0);
			if (wstrSize == 0)
				BASE_THROW(WindowsError());

			std::wstring wstr(wstrSize, L'\0');
			int ret = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
				ptr, -1, &wstr[0], wstrSize);
			if (ret == 0)
				BASE_THROW(WindowsError());

			return wstr;
		}

		std::wstring Apis::Utf8ToWide(const std::string& str)
		{
			if (str.empty())
				return std::wstring();

			int wstrSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
				str.data(), str.size(), nullptr, 0);
			if (wstrSize == 0)
				BASE_THROW(WindowsError());

			std::wstring wstr(wstrSize, L'\0');
			int ret = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
				str.data(), str.size(), &wstr[0], wstrSize);
			if (ret == 0)
				BASE_THROW(WindowsError());

			return wstr;
		}

		std::string Apis::WideToUtf8(const wchar_t* wptr)
		{
			if (wptr == nullptr || wptr[0] == L'\0')
				return std::string();

			int strSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
				wptr, -1, nullptr, 0, nullptr, nullptr);
			if (strSize == 0)
				BASE_THROW(WindowsError());

			std::string str(strSize, '\0');
			int ret = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
				wptr, -1, &str[0], strSize, nullptr, nullptr);
			if (ret == 0)
				BASE_THROW(WindowsError());

			return str;
		}

		std::string Apis::WideToUtf8(const std::wstring& wstr)
		{
			if (wstr.empty())
				return std::string();

			int strSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
				wstr.data(), wstr.size(), nullptr, 0, nullptr, nullptr);
			if (strSize == 0)
				BASE_THROW(WindowsError());

			std::string str(strSize, '\0');
			int ret = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
				wstr.data(), wstr.size(), &str[0], strSize, nullptr, nullptr);
			if (ret == 0)
				BASE_THROW(WindowsError());

			return str;
		}

		std::string Apis::GetErrorDesc(DWORD errorId)
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, errorId, 0, buffer, BUFFER_SIZE - 1, nullptr);
			if (ret == 0)
				BASE_THROW(WindowsError());

			return WideToUtf8(buffer);
		}

		std::string Apis::GetModuleName(HMODULE module)
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				BASE_THROW(WindowsError());

			return WideToUtf8(buffer);
		}

		std::string Apis::GetModuleDir(HMODULE module)
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				BASE_THROW(WindowsError());

			filesystem::wpath p(buffer);
			return WideToUtf8(p.remove_filename().wstring());
		}
	}
}

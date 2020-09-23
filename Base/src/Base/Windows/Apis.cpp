#include "Base/Windows/Apis.h"

#include <boost/filesystem.hpp>

#include "Base/Windows/WindowsError.h"

namespace base
{
	namespace win
	{
		namespace filesystem = boost::filesystem;

		std::string Apis::GetErrorDesc(DWORD errorId)
		{
			WCHAR buffer[2048] = {};
			if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, errorId, 0, buffer, 2047, nullptr) == 0)
				return "FormatMessage() failed.";
			return String::WideToUtf8(buffer);
		}

		std::string Apis::GetModuleName(HMODULE module)
		{
			WCHAR buffer[2048] = {};
			if (GetModuleFileNameW(module, buffer, 2047) == 0)
				BASE_THROW(WindowsError());
			return String::WideToUtf8(buffer);
		}

		std::string Apis::GetModuleDir(HMODULE module)
		{
			WCHAR buffer[2048] = {};
			if (GetModuleFileNameW(module, buffer, 2047) == 0)
				BASE_THROW(WindowsError());
			filesystem::wpath fullPath(buffer);
			return String::WideToUtf8(fullPath.remove_filename().wstring());
		}
	}
}

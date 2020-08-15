#include "Windows/Common.h"
#include "Windows/Apis.h"

#include <boost/filesystem.hpp>

#include "Windows/WindowsError.h"

namespace win
{
	using namespace boost::filesystem;

	string Apis::GetErrorDesc(DWORD errorId)
	{
		WCHAR buffer[4096] = {};
		if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorId, 0, buffer, 4095, nullptr) == 0)
			return "FormatMessage() failed.";
		return String::WideToUtf8(buffer);
	}

	string Apis::GetModuleName(HMODULE module)
	{
		WCHAR buffer[4096] = {};
		if (GetModuleFileNameW(module, buffer, 4095) == 0)
			BASE_THROW(WindowsError());
		return String::WideToUtf8(buffer);
	}

	string Apis::GetModuleDir(HMODULE module)
	{
		WCHAR buffer[4096] = {};
		if (GetModuleFileNameW(module, buffer, 4095) == 0)
			BASE_THROW(WindowsError());
		wpath fullPath(buffer);
		return String::WideToUtf8(fullPath.remove_filename().wstring());
	}
}

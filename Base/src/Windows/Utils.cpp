#include "Windows/Common.h"
#include "Windows/Utils.h"

#include <boost/filesystem.hpp>

#include "Windows/WindowsError.h"

namespace win
{
	namespace bfs = boost::filesystem;

	std::string Utils::GetErrorDescription(DWORD errorId)
	{
		WCHAR buffer[4096] = {};
		if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorId, 0, buffer, 4095, nullptr) == 0)
			return "FormatMessageW() failed.";
		return String::WideToUtf8(buffer);
	}

	std::string Utils::GetModuleName(HMODULE module)
	{
		WCHAR buffer[4096] = {};
		if (GetModuleFileNameW(module, buffer, 4095) == 0)
			BASE_THROW(WindowsError(GetLastError()));
		return String::WideToUtf8(buffer);
	}

	std::string Utils::GetModuleDir(HMODULE module)
	{
		WCHAR buffer[4096] = {};
		if (GetModuleFileNameW(module, buffer, 4095) == 0)
			BASE_THROW(WindowsError(GetLastError()));
		bfs::path path(buffer);
		return String::WideToUtf8(path.remove_filename().wstring());
	}
}

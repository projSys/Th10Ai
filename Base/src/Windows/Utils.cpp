#include "Windows/Common.h"
#include "Windows/Utils.h"

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

#include "Windows/WindowsError.h"

namespace win
{
	namespace bfs = boost::filesystem;
	namespace blc = boost::locale::conv;

	std::string Utils::GetErrorDescription(DWORD errorId)
	{
		WCHAR buffer[4096] = {};
		if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorId, 0, buffer, 4095, nullptr) == 0)
			return "FormatMessageW() failed.";
		return blc::utf_to_utf<char>(buffer);
	}

	std::string Utils::GetModuleName(HMODULE module)
	{
		WCHAR buffer[4096] = {};
		if (GetModuleFileNameW(module, buffer, 4095) == 0)
			BASE_THROW(WindowsError(GetLastError()));
		return blc::utf_to_utf<char>(buffer);
	}

	std::string Utils::GetModuleDir(HMODULE module)
	{
		WCHAR buffer[4096] = {};
		if (GetModuleFileNameW(module, buffer, 4095) == 0)
			BASE_THROW(WindowsError(GetLastError()));
		bfs::path path(buffer);
		return blc::utf_to_utf<char>(path.remove_filename().wstring());
	}
}

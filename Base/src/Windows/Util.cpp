#include "Windows/Common.h"
#include "Windows/Util.h"

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

#include "Windows/WindowsError.h"

namespace win
{
	namespace bfs = boost::filesystem;
	namespace blc = boost::locale::conv;

	std::string Util::GetErrorMessage(DWORD messageId)
	{
		TCHAR buffer[4096] = {};
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, messageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer, 4095, nullptr);
		return blc::utf_to_utf<char>(buffer);
	}

	std::string Util::GetModuleName(HMODULE module)
	{
		TCHAR buffer[4096] = {};
		if (GetModuleFileName(module, buffer, 4095) == 0)
			THROW_WINDOWS_ERROR(GetLastError());
		return blc::utf_to_utf<char>(buffer);
	}

	std::string Util::GetModuleDir(HMODULE module)
	{
		TCHAR buffer[4096] = {};
		if (GetModuleFileName(module, buffer, 4095) == 0)
			THROW_WINDOWS_ERROR(GetLastError());
		bfs::path path(buffer);
		return blc::utf_to_utf<char>(path.remove_filename().wstring());
	}
}

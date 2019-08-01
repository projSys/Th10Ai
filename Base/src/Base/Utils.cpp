#include "Base/Common.h"
#include "Base/Utils.h"

#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

namespace base
{
	namespace bfs = boost::filesystem;
	namespace blc = boost::locale::conv;

	std::string Utils::GetErrorMessage(DWORD messageId)
	{
		TCHAR buffer[4096] = {};
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, messageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer, 4095, nullptr);
		return blc::utf_to_utf<char>(buffer);
	}

	std::string Utils::HresultToString(HRESULT hr)
	{
		std::ostringstream oss;
		oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << hr;
		return oss.str();
	}

	std::string Utils::GetModulePath(HMODULE module)
	{
		TCHAR buffer[4096] = {};
		if (GetModuleFileName(module, buffer, 4095) == 0)
			THROW_WINDOWS_ERROR(GetLastError());
		return blc::utf_to_utf<char>(buffer);
	}

	std::string Utils::GetModuleDir(HMODULE module)
	{
		TCHAR buffer[4096] = {};
		if (GetModuleFileName(module, buffer, 4095) == 0)
			THROW_WINDOWS_ERROR(GetLastError());
		bfs::path path(buffer);
		return blc::utf_to_utf<char>(path.remove_filename().wstring());
	}
}

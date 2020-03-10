#pragma once

namespace win
{
	class Util
	{
	public:
		static std::string GetErrorDescription(DWORD errorId);
		static std::string GetModuleName(HMODULE module = nullptr);
		static std::string GetModuleDir(HMODULE module = nullptr);
	};
}

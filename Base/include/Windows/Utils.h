#pragma once

namespace win
{
	class Utils
	{
	public:
		static string GetErrorDescription(DWORD errorId);
		static string GetModuleName(HMODULE module = nullptr);
		static string GetModuleDir(HMODULE module = nullptr);
	};
}

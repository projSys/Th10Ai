#pragma once

namespace win
{
	class Apis
	{
	public:
		static string GetErrorDesc(DWORD errorId);
		static string GetModuleName(HMODULE module = nullptr);
		static string GetModuleDir(HMODULE module = nullptr);
	};
}

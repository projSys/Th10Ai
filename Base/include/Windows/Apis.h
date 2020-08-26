#pragma once

#include "Windows/Common.h"

namespace win
{
	class Apis
	{
	public:
		static std::string GetErrorDesc(DWORD errorId);
		static std::string GetModuleName(HMODULE module = nullptr);
		static std::string GetModuleDir(HMODULE module = nullptr);
	};
}

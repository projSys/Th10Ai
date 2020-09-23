#pragma once

#include "Base/Windows/Common.h"

namespace base
{
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
}

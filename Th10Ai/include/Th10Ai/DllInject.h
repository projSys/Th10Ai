#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(DWORD processId, const std::string& dllName);
	};
}

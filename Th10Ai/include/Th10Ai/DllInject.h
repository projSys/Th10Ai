#pragma once

#include "Th10Ai/Common.h"

#include <Base/Windows/Process.h>

namespace th
{
	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(Process& target, const std::string& dllPath);
	};
}

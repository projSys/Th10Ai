#pragma once

#include <Base/Process.h>

namespace gc
{
	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(Process& target, const std::string& dllName);
		static void Uninject(Process& target, const std::string& dllName);
	};
}

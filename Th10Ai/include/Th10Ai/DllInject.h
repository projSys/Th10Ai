#pragma once

#include <Windows/Process.h>

namespace th
{
	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(Process& target, const std::string& dllName);
	};
}

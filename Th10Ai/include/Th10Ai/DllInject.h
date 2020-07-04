#pragma once

namespace th
{
	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(DWORD processId, const string& dllName);
	};
}

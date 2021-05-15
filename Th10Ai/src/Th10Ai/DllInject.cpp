#include "Th10Ai/DllInject.h"

#include <system_error>
#include <Base/ScopeGuard.h>
#include <Base/Windows/Handle.h>
#include <Base/Windows/Module.h>
#include <Base/Windows/Thread.h>

namespace th
{
	void DllInject::EnableDebugPrivilege()
	{
		Handle token;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			BASE_THROW(Exception(u8"请以管理员身份运行。"));
	}

	void DllInject::Inject(Process& process, const fs::path& dllPath)
	{
		uint_t size = sizeof(fs::path::value_type) * (dllPath.size() + 1);

		LPVOID memory = VirtualAllocEx(process, nullptr, size, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (memory == nullptr)
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process, memory, 0, MEM_RELEASE);
			});

		SIZE_T written = 0;
		if (!WriteProcessMemory(process, memory, dllPath.c_str(), size, &written))
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));
		if (written != size)
			BASE_THROW(Exception("written != size."));

		Module kernel32Dll = Module::Get("kernel32.dll");
		LPTHREAD_START_ROUTINE loadLibraryW =
			kernel32Dll.getProcAddress<LPTHREAD_START_ROUTINE>("LoadLibraryW");

		Thread thread = process.createRemoteThread(nullptr, 0, loadLibraryW, memory, 0);
		if (!thread.wait(3000))
			BASE_THROW(Exception(u8"注入线程执行超时。"));
		if (thread.getExitCode() == 0)
			BASE_THROW(Exception(u8"LoadLibraryW()调用失败。"));
	}
}

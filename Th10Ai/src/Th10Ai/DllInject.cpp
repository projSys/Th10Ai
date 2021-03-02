#include "Th10Ai/DllInject.h"

#include <Base/ScopeGuard.h>
#include <Base/Windows/WindowsError.h>
#include <Base/Windows/Apis.h>
#include <Base/Windows/Handle.h>
#include <Base/Windows/Module.h>
#include <Base/Windows/Thread.h>

namespace th
{
	void DllInject::EnableDebugPrivilege()
	{
		Handle token;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			BASE_THROW(WindowsError());

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			BASE_THROW(WindowsError());

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			BASE_THROW(WindowsError());
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			BASE_THROW(Exception(u8"请以管理员身份运行。"));
	}

	void DllInject::Inject(Process& target, const std::string& dllPath)
	{
		std::wstring dllPathW = Apis::Utf8ToWide(dllPath);
		uint_t sizeW = sizeof(wchar_t) * (dllPathW.size() + 1);

		LPVOID memory = VirtualAllocEx(target, nullptr, sizeW, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (memory == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(target, memory, 0, MEM_RELEASE);
			});

		SIZE_T written = 0;
		if (!WriteProcessMemory(target, memory, dllPathW.c_str(), sizeW, &written))
			BASE_THROW(WindowsError());
		if (written != sizeW)
			BASE_THROW(Exception("written != sizeW."));

		Module kernel32Dll = Module::Get("kernel32.dll");
		LPTHREAD_START_ROUTINE loadLibraryW =
			kernel32Dll.getProcAddress<LPTHREAD_START_ROUTINE>("LoadLibraryW");

		Thread thread = target.createRemoteThread(nullptr, 0, loadLibraryW, memory, 0);
		if (!thread.wait(3000))
			BASE_THROW(Exception(u8"注入线程执行超时。"));
		if (thread.getExitCode() == 0)
			BASE_THROW(Exception(u8"LoadLibraryW()调用失败。"));
	}
}

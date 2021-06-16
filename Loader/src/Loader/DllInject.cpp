#include "Loader/DllInject.h"

#include <Base/Exception.h>
#include <Base/ScopeGuard.h>

namespace ld
{
	void DllInject::EnableDebugPrivilege()
	{
		HANDLE token = nullptr;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
			&token))
			BASE_THROW(SystemError, GetLastError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(token);
			});

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			BASE_THROW(SystemError, GetLastError());

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			BASE_THROW(SystemError, GetLastError());
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			BASE_THROW(Exception, u8"请以管理员身份运行。");
	}

	void DllInject::Inject(HANDLE process, const fs::path& dllPath)
	{
		uint_t size = sizeof(fs::path::value_type) * (dllPath.size() + 1);

		LPVOID memory = VirtualAllocEx(process, nullptr, size, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (memory == nullptr)
			BASE_THROW(SystemError, GetLastError());
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process, memory, 0, MEM_RELEASE);
			});

		SIZE_T written = 0;
		if (!WriteProcessMemory(process, memory, dllPath.c_str(), size, &written))
			BASE_THROW(SystemError, GetLastError());
		if (written != size)
			BASE_THROW(Exception, "written != size.");

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			BASE_THROW(SystemError, GetLastError());
		LPTHREAD_START_ROUTINE loadLibraryW =
			reinterpret_cast<LPTHREAD_START_ROUTINE>(
				GetProcAddress(kernel32Dll, "LoadLibraryW"));
		if (loadLibraryW == nullptr)
			BASE_THROW(SystemError, GetLastError());

		DWORD threadId = 0;
		HANDLE thread = CreateRemoteThread(process, nullptr, 0, loadLibraryW, memory,
			0, &threadId);
		if (thread == nullptr)
			BASE_THROW(SystemError, GetLastError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(thread);
			});

		DWORD waited = WaitForSingleObject(thread, 3000);
		if (waited == WAIT_FAILED)
			BASE_THROW(SystemError, GetLastError());
		if (waited == WAIT_TIMEOUT)
			BASE_THROW(Exception, u8"注入线程执行超时。");

		DWORD exitCode = 0;
		if (!GetExitCodeThread(thread, &exitCode))
			BASE_THROW(SystemError, GetLastError());
		if (exitCode == 0)
			BASE_THROW(Exception, u8"LoadLibraryW()调用失败。");
	}
}

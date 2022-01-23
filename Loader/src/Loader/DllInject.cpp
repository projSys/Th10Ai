#include "Loader/DllInject.h"

#include <Base/ScopeGuard.h>
#include <Base/Exception.h>
#include <Base/Windows/SystemError.h>

namespace ld
{
	void DllInject::EnableDebugPrivilege()
	{
		HANDLE token = nullptr;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			throw SystemError();
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(token);
			});

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			throw SystemError();

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			throw SystemError(GetLastError());
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			throw Exception("请以管理员身份运行。");
	}

	void DllInject::Inject(HANDLE process, const fs::path& dllPath)
	{
		uint_t size = sizeof(fs::path::value_type) * (dllPath.size() + 1);

		LPVOID memory = VirtualAllocEx(process, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (memory == nullptr)
			throw SystemError();
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process, memory, 0, MEM_RELEASE);
			});

		SIZE_T written = 0;
		if (!WriteProcessMemory(process, memory, dllPath.c_str(), size, &written))
			throw SystemError();
		if (written != size)
			throw Exception("written != size.");

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			throw SystemError();
		FARPROC loadLibraryW = GetProcAddress(kernel32Dll, "LoadLibraryW");
		if (loadLibraryW == nullptr)
			throw SystemError();

		DWORD threadId = 0;
		HANDLE thread = CreateRemoteThread(process, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryW), memory, 0, &threadId);
		if (thread == nullptr)
			throw SystemError();
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(thread);
			});

		DWORD waited = WaitForSingleObject(thread, 3000);
		if (waited == WAIT_FAILED)
			throw SystemError();
		if (waited == WAIT_TIMEOUT)
			throw Exception("注入线程执行超时。");

		DWORD exitCode = 0;
		if (!GetExitCodeThread(thread, &exitCode))
			throw SystemError();
		if (exitCode == 0)
			throw Exception("LoadLibraryW()调用失败。");
	}
}

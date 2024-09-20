#include "Th10Ai/DllInject.h"

#include "Base/ScopeGuard.h"
#include "Base/Exception/Exception.h"
#include "Base/Exception/SystemError.h"

namespace th
{
	void DllInject::EnableDebugPrivilege()
	{
		HANDLE token = nullptr;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			throw SystemError(GetLastError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(token);
			});

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			throw SystemError(GetLastError());

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
		SIZE_T size = sizeof(fs::path::value_type) * (dllPath.native().size() + 1);

		LPVOID memory = VirtualAllocEx(process, nullptr, size, MEM_COMMIT, PAGE_READWRITE);
		if (memory == nullptr)
			throw SystemError(GetLastError());
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process, memory, 0, MEM_RELEASE);
			});

		if (!WriteProcessMemory(process, memory, dllPath.c_str(), size, nullptr))
			throw SystemError(GetLastError());

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			throw SystemError(GetLastError());
		LPTHREAD_START_ROUTINE loadLibraryW = reinterpret_cast<LPTHREAD_START_ROUTINE>(
			GetProcAddress(kernel32Dll, "LoadLibraryW"));
		if (loadLibraryW == nullptr)
			throw SystemError(GetLastError());

		HANDLE thread = CreateRemoteThread(process, nullptr, 0, loadLibraryW, memory, 0, nullptr);
		if (thread == nullptr)
			throw SystemError(GetLastError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(thread);
			});

		DWORD waited = WaitForSingleObject(thread, 3000);
		if (waited == WAIT_FAILED)
			throw SystemError(GetLastError());
		if (waited == WAIT_TIMEOUT)
			throw Exception("注入线程执行超时。");

		DWORD exitCode = 0;
		if (!GetExitCodeThread(thread, &exitCode))
			throw SystemError(GetLastError());
		if (exitCode == 0)
			throw Exception("LoadLibraryW()调用失败。");
	}
}

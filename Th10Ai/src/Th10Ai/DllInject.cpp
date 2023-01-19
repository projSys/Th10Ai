#include "Th10Ai/DllInject.h"

#include <Base/ScopeGuard.h>
#include <Base/Exception.h>
#include <Base/Windows/WindowsError.h>

namespace th
{
	void DllInject::EnableDebugPrivilege()
	{
		HANDLE token = nullptr;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			Throw(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(token);
			});

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			Throw(WindowsError(GetLastError()));

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			Throw(WindowsError(GetLastError()));
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			Throw(Exception("请以管理员身份运行。"));
	}

	void DllInject::Inject(HANDLE process, const fs::path& dllPath)
	{
		uint_t size = sizeof(fs::path::value_type) * (dllPath.native().size() + 1);

		LPVOID memory = VirtualAllocEx(process, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (memory == nullptr)
			Throw(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process, memory, 0, MEM_RELEASE);
			});

		SIZE_T written = 0;
		if (!WriteProcessMemory(process, memory, dllPath.c_str(), size, &written))
			Throw(WindowsError(GetLastError()));
		if (written != size)
			Throw(Exception("written != size."));

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			Throw(WindowsError(GetLastError()));
		FARPROC loadLibraryW = GetProcAddress(kernel32Dll, "LoadLibraryW");
		if (loadLibraryW == nullptr)
			Throw(WindowsError(GetLastError()));

		DWORD threadId = 0;
		HANDLE thread = CreateRemoteThread(process, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryW), memory, 0, &threadId);
		if (thread == nullptr)
			Throw(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(thread);
			});

		DWORD waited = WaitForSingleObject(thread, 3000);
		if (waited == WAIT_FAILED)
			Throw(WindowsError(GetLastError()));
		if (waited == WAIT_TIMEOUT)
			Throw(Exception("注入线程执行超时。"));

		DWORD exitCode = 0;
		if (!GetExitCodeThread(thread, &exitCode))
			Throw(WindowsError(GetLastError()));
		if (exitCode == 0)
			Throw(Exception("LoadLibraryW()调用失败。"));
	}

	void DllInject::Launch(const fs::path& exePath, const fs::path& dllPath)
	{
		fs::path exeDir = exePath.parent_path();

		STARTUPINFOW si = {};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi = {};
		if (!CreateProcessW(exePath.c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, exeDir.c_str(), &si, &pi))
			Throw(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&pi]()
			{
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			});

		//EnableDebugPrivilege();
		Inject(pi.hProcess, dllPath);

		DWORD count = ResumeThread(pi.hThread);
		if (count == (DWORD)-1)
			Throw(WindowsError(GetLastError()));
	}
}

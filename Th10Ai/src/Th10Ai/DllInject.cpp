#include "Th10Ai/Common.h"
#include "Th10Ai/DllInject.h"

#include <Base/ScopeGuard.h>

namespace th
{
	void DllInject::EnableDebugPrivilege()
	{
		HANDLE token = nullptr;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(token);
			});

		LUID luid = {};
		if (!LookupPrivilegeValueW(nullptr, SE_DEBUG_NAME, &luid))
			BASE_THROW(WindowsError());

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			BASE_THROW(WindowsError());
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			BASE_THROW(Exception("Please run as administrator."));
	}

	void DllInject::Inject(DWORD processId, const string& dllName)
	{
		wstring dllNameW = String::Utf8ToWide(dllName);
		uint_t size = (dllNameW.length() + 1) * sizeof(wchar_t);

		HANDLE_ptr process(OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId),
			&CloseHandle);
		if (process == nullptr)
			BASE_THROW(WindowsError());

		LPVOID remoteMemory = VirtualAllocEx(process.get(), nullptr, size, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (remoteMemory == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process.get(), remoteMemory, 0, MEM_RELEASE);
			});

		if (!WriteProcessMemory(process.get(), remoteMemory, dllNameW.c_str(), size, nullptr))
			BASE_THROW(WindowsError());

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			BASE_THROW(WindowsError());
		FARPROC loadLibraryW = GetProcAddress(kernel32Dll, "LoadLibraryW");
		if (loadLibraryW == nullptr)
			BASE_THROW(WindowsError());

		HANDLE_ptr thread(CreateRemoteThread(process.get(), nullptr, 0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryW), remoteMemory, 0, nullptr),
			&CloseHandle);
		if (thread == nullptr)
			BASE_THROW(WindowsError());

		WaitForSingleObject(thread.get(), INFINITE);

		// 获取线程退出码，即LoadLibraryW的返回值dll句柄
		DWORD exitCode = 0;
		GetExitCodeThread(thread.get(), &exitCode);
		if (exitCode == 0)
			BASE_THROW(Exception("LoadLibraryW() failed."));
	}
}

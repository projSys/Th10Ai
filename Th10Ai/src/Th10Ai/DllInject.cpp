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

	void DllInject::Inject(DWORD processId, const std::string& dllName)
	{
		std::wstring dllNameW = String::Utf8ToWide(dllName);
		uint_t size = (dllNameW.length() + 1) * sizeof(wchar_t);

		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		if (process == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(process);
			});

		LPVOID memory = VirtualAllocEx(process, nullptr, size, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (memory == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process, memory, 0, MEM_RELEASE);
			});

		SIZE_T written = 0;
		if (!WriteProcessMemory(process, memory, dllNameW.c_str(), size, &written))
			BASE_THROW(WindowsError());
		if (written != size)
			BASE_THROW(Exception("written != size."));

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			BASE_THROW(WindowsError());
		FARPROC loadLibraryW = GetProcAddress(kernel32Dll, "LoadLibraryW");
		if (loadLibraryW == nullptr)
			BASE_THROW(WindowsError());

		HANDLE thread = CreateRemoteThread(process, nullptr, 0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryW), memory, 0, nullptr);
		if (thread == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(thread);
			});

		DWORD ret = WaitForSingleObject(thread, 5000);
		if (ret == WAIT_FAILED)
			BASE_THROW(WindowsError());
		if (ret == WAIT_TIMEOUT)
			BASE_THROW(Exception(u8"远线程执行超时。"));

		// 获取线程退出码，即LoadLibraryW的返回值dll句柄
		DWORD exitCode = 0;
		if (!GetExitCodeThread(thread, &exitCode))
			BASE_THROW(WindowsError());
		if (exitCode == 0)
			BASE_THROW(Exception(u8"LoadLibraryW()调用失败。"));
	}
}

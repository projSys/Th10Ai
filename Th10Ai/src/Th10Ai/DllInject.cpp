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

	void _Inject(HANDLE process, const std::string& funcName, const std::string& param)
	{
		std::wstring paramW = Apis::Utf8ToWide(param);
		uint_t sizeW = sizeof(std::wstring::value_type) * (paramW.size() + 1);

		LPVOID memory = VirtualAllocEx(process, nullptr, sizeW, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (memory == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(process, memory, 0, MEM_RELEASE);
			});

		SIZE_T written = 0;
		if (!WriteProcessMemory(process, memory, paramW.c_str(), sizeW, &written))
			BASE_THROW(WindowsError());
		if (written != sizeW)
			BASE_THROW(Exception("written != sizeW."));

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			BASE_THROW(WindowsError());
		FARPROC func = GetProcAddress(kernel32Dll, funcName.c_str());
		if (SetDllDirectoryW == nullptr)
			BASE_THROW(WindowsError());

		HANDLE thread = CreateRemoteThread(process, nullptr, 0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(func), memory, 0, nullptr);
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

		DWORD exitCode = 0;
		if (!GetExitCodeThread(thread, &exitCode))
			BASE_THROW(WindowsError());
		if (exitCode == 0)
			BASE_THROW(Exception(u8"远函数调用失败。"));
	}

	void DllInject::Inject(DWORD processId, const std::string& dllPath)
	{
		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		if (process == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(process);
			});

		_Inject(process, "LoadLibraryW", dllPath);
	}

	void DllInject::Inject(DWORD processId, const std::string& dllDir,
		const std::string& dllName)
	{
		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		if (process == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(process);
			});

		_Inject(process, "SetDllDirectoryW", dllDir);
		_Inject(process, "LoadLibraryW", dllName);
	}
}

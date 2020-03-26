#include "Th10Ai/Common.h"
#include "Th10Ai/DllInject.h"

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <Base/ScopeGuard.h>

namespace th
{
	namespace bfs = boost::filesystem;
	namespace blc = boost::locale::conv;

	void DllInject::EnableDebugPrivilege()
	{
		HANDLE token = nullptr;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			BASE_THROW(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(token);
			});

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			BASE_THROW(WindowsError(GetLastError()));

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			BASE_THROW(WindowsError(GetLastError()));
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			BASE_THROW(Exception("Please run as administrator."));
	}

	void DllInject::Inject(Process& target, const std::string& dllName)
	{
		std::wstring dllNameW = blc::utf_to_utf<wchar_t>(dllName);
		uint_t size = (dllNameW.length() + 1) * sizeof(wchar_t);

		if (!bfs::exists(dllNameW))
			BASE_THROW(Exception("File not found: " + dllName));

		LPVOID remoteMemory = VirtualAllocEx(target, nullptr, size, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (remoteMemory == nullptr)
			BASE_THROW(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
			{
				VirtualFreeEx(target, remoteMemory, 0, MEM_RELEASE);
			});

		if (!WriteProcessMemory(target, remoteMemory, dllNameW.c_str(), size, nullptr))
			BASE_THROW(WindowsError(GetLastError()));

		HMODULE kernel32Dll = GetModuleHandleW(L"kernel32.dll");
		if (kernel32Dll == nullptr)
			BASE_THROW(WindowsError(GetLastError()));
		FARPROC loadLibrary = GetProcAddress(kernel32Dll, "LoadLibraryW");
		if (loadLibrary == nullptr)
			BASE_THROW(WindowsError(GetLastError()));

		DWORD threadId = 0;
		HANDLE thread = CreateRemoteThread(target, nullptr, 0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibrary), remoteMemory, 0, &threadId);
		if (thread == nullptr)
			BASE_THROW(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(thread);
			});
		WaitForSingleObject(thread, INFINITE);
	}
}

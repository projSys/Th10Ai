#include "GraphCap/Common.h"
#include "GraphCap/DllInject.h"

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <Base/ScopeGuard.h>

namespace gc
{
	namespace bfs = boost::filesystem;
	namespace blc = boost::locale::conv;

	void DllInject::EnableDebugPrivilege()
	{
		HANDLE token = nullptr;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			CloseHandle(token);
		});

		LUID luid = {};
		if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
			THROW_WINDOWS_ERROR(GetLastError());

		TOKEN_PRIVILEGES tp = {};
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
			THROW_WINDOWS_ERROR(GetLastError());
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			THROW_BASE_EXCEPTION(Exception() << err_str(u8"请以管理员身份运行。"));
	}

	void DllInject::Inject(Process& target, const std::string& dllName)
	{
		std::wstring dllNameW = blc::utf_to_utf<wchar_t>(dllName);
		uint_t dllNameSizeW = (dllNameW.length() + 1) * sizeof(wchar_t);

		if (!bfs::exists(dllNameW))
			THROW_BASE_EXCEPTION(Exception() << err_str(u8"文件不存在：" + dllName));

		LPVOID remoteMemory = VirtualAllocEx(target, nullptr, dllNameSizeW, MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
		if (remoteMemory == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			VirtualFreeEx(target, remoteMemory, 0, MEM_RELEASE);
		});

		if (!WriteProcessMemory(target, remoteMemory, dllNameW.c_str(), dllNameSizeW, nullptr))
			THROW_WINDOWS_ERROR(GetLastError());

		HMODULE kernel32Dll = GetModuleHandle(_T("kernel32.dll"));
		if (kernel32Dll == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		FARPROC loadLibrary = GetProcAddress(kernel32Dll, "LoadLibraryW");
		if (loadLibrary == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());

		HANDLE remoteThread = CreateRemoteThread(target, nullptr, 0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibrary), remoteMemory, 0, nullptr);
		if (remoteThread == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			CloseHandle(remoteThread);
		});
		WaitForSingleObject(remoteThread, INFINITE);
	}

	void DllInject::Uninject(Process& target, const std::string& dllName)
	{
		HMODULE module = target.findModuleByName(dllName);
		if (module == nullptr)
			return;

		HMODULE kernel32Dll = GetModuleHandle(_T("kernel32.dll"));
		if (kernel32Dll == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		FARPROC freeLibrary = GetProcAddress(kernel32Dll, "FreeLibrary");
		if (freeLibrary == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());

		HANDLE remoteThread = CreateRemoteThread(target, nullptr, 0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(freeLibrary), module, 0, nullptr);
		if (remoteThread == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			CloseHandle(remoteThread);
		});
		WaitForSingleObject(remoteThread, INFINITE);
	}
}

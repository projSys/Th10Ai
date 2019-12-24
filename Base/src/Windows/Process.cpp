#include "Windows/Common.h"
#include "Windows/Process.h"

#include <TlHelp32.h>
#include <boost/locale.hpp>

#include "Base/ScopeGuard.h"
#include "Windows/WindowsError.h"
#include "Windows/Util.h"

namespace win
{
	namespace blc = boost::locale::conv;

	Process Process::FindByName(const std::string& name,
		DWORD desiredAccess, BOOL inheritHandle)
	{
		std::wstring nameW = blc::utf_to_utf<wchar_t>(name);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE)
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
		{
			CloseHandle(snapshot);
		});

		DWORD id = INVALID_ID;
		PROCESSENTRY32W pe = {};
		pe.dwSize = sizeof(pe);
		for (BOOL hasNext = Process32FirstW(snapshot, &pe); hasNext;
			hasNext = Process32NextW(snapshot, &pe))
		{
			if (pe.szExeFile == nameW)
			{
				id = pe.th32ProcessID;
				break;
			}
		}
		if (id == INVALID_ID)
			BASE_THROW_EXCEPTION(Exception("Process not found: " + name));
		return Process(id, desiredAccess, inheritHandle);
	}

	Process::Process() :
		m_process(nullptr),
		m_id(INVALID_ID)
	{
	}

	Process::Process(DWORD id, DWORD desiredAccess, BOOL inheritHandle) :
		m_process(nullptr),
		m_id(id)
	{
		m_process = OpenProcess(desiredAccess, inheritHandle, id);
		if (m_process == nullptr)
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
	}

	Process::~Process()
	{
		if (m_process != nullptr)
			CloseHandle(m_process);
	}

	Process::Process(Process&& other) :
		m_process(other.m_process),
		m_id(other.m_id)
	{
		other.m_process = nullptr;
		other.m_id = INVALID_ID;
	}

	Process& Process::operator =(Process&& other)
	{
		Process(std::move(other)).swap(*this);
		return *this;
	}

	void Process::swap(Process& other)
	{
		std::swap(m_process, other.m_process);
		std::swap(m_id, other.m_id);
	}

	HMODULE Process::findModuleByName(const std::string& moduleName) const
	{
		std::wstring moduleNameW = blc::utf_to_utf<wchar_t>(moduleName);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_id);
		if (snapshot == INVALID_HANDLE_VALUE)
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		ON_SCOPE_EXIT([&]()
		{
			CloseHandle(snapshot);
		});

		HMODULE module = nullptr;
		MODULEENTRY32W me = {};
		me.dwSize = sizeof(me);
		for (BOOL hasNext = Module32FirstW(snapshot, &me); hasNext;
			hasNext = Module32NextW(snapshot, &me))
		{
			if (me.szModule == moduleNameW)
			{
				module = me.hModule;
				break;
			}
		}
		return module;
	}

	bool Process::isWow64() const
	{
		BOOL ret = FALSE;
		if (!IsWow64Process(m_process, &ret))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		return ret;
	}

	bool Process::isActive() const
	{
		DWORD exitCode = 0;
		if (!GetExitCodeProcess(m_process, &exitCode))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		return exitCode == STILL_ACTIVE;
	}

	Process::operator HANDLE() const
	{
		return m_process;
	}

	DWORD Process::getId() const
	{
		return m_id;
	}
}

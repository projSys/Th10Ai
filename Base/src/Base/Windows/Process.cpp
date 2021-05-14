#include "Base/Windows/Process.h"

#include "Base/Windows/WindowsError.h"
#include "Base/Windows/Thread.h"

namespace base
{
	namespace win
	{
		std::pair<Process, Thread> Process::Create(const fs::path& applicationName,
			LPWSTR commandLine, LPSECURITY_ATTRIBUTES processAttributes,
			LPSECURITY_ATTRIBUTES threadAttributes, BOOL inheritHandles,
			DWORD creationFlags, LPVOID environment, const fs::path& currentDirectory,
			LPSTARTUPINFOW startupInfo)
		{
			STARTUPINFOW si = {};
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi = {};

			if (startupInfo == nullptr)
				startupInfo = &si;

			if (!CreateProcessW(applicationName.c_str(), commandLine, processAttributes,
				threadAttributes, inheritHandles, creationFlags, environment,
				currentDirectory.c_str(), startupInfo, &pi))
				BASE_THROW(WindowsError());
			return std::make_pair(Process(pi.hProcess), Thread(pi.hThread));
		}

		Process Process::Open(DWORD desiredAccess, BOOL inheritHandle, DWORD processId)
		{
			Process process(OpenProcess(desiredAccess, inheritHandle, processId));
			if (process == nullptr)
				BASE_THROW(WindowsError());
			return process;
		}

		Thread Process::createRemoteThread(LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize,
			LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD creationFlags)
		{
			DWORD threadId = 0;
			Thread thread(CreateRemoteThread(m_handle, threadAttributes, stackSize,
				startAddress, parameter, creationFlags, &threadId));
			if (thread == nullptr)
				BASE_THROW(WindowsError());
			return thread;
		}

		DWORD Process::getId() const
		{
			DWORD id = GetProcessId(m_handle);
			if (id == 0)
				BASE_THROW(WindowsError());
			return id;
		}

		DWORD Process::getExitCode() const
		{
			DWORD exitCode = 0;
			if (!GetExitCodeProcess(m_handle, &exitCode))
				BASE_THROW(WindowsError());
			return exitCode;
		}
	}
}

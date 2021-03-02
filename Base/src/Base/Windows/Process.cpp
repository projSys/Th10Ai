#include "Base/Windows/Process.h"

#include "Base/Windows/WindowsError.h"

namespace base
{
	namespace win
	{
		//Process Process::Create()
		//{
		//	CreateProcessW();
		//}

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

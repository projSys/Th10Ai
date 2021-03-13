#include "Base/Windows/Thread.h"

#include "Base/Windows/WindowsError.h"

namespace base
{
	namespace win
	{
		Thread Thread::Create(LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize,
			LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD creationFlags)
		{
			DWORD threadId = 0;
			Thread thread(CreateThread(threadAttributes, stackSize,
				startAddress, parameter, creationFlags, &threadId));
			if (thread == nullptr)
				BASE_THROW(WindowsError());
			return thread;
		}

		Thread Thread::Open(DWORD desiredAccess, BOOL inheritHandle, DWORD threadId)
		{
			Thread thread(OpenThread(desiredAccess, inheritHandle, threadId));
			if (thread == nullptr)
				BASE_THROW(WindowsError());
			return thread;
		}

		DWORD Thread::resume()
		{
			DWORD count = ResumeThread(m_handle);
			if (count == (DWORD)-1)
				BASE_THROW(WindowsError());
			return count;
		}

		DWORD Thread::getId() const
		{
			DWORD id = GetThreadId(m_handle);
			if (id == 0)
				BASE_THROW(WindowsError());
			return id;
		}

		DWORD Thread::getExitCode() const
		{
			DWORD exitCode = 0;
			if (!GetExitCodeThread(m_handle, &exitCode))
				BASE_THROW(WindowsError());
			return exitCode;
		}
	}
}

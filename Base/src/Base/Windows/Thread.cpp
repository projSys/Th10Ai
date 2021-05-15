#include "Base/Windows/Thread.h"

#include <system_error>

#include "Base/Exception.h"

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
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return thread;
		}

		Thread Thread::Open(DWORD desiredAccess, BOOL inheritHandle, DWORD threadId)
		{
			Thread thread(OpenThread(desiredAccess, inheritHandle, threadId));
			if (thread == nullptr)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return thread;
		}

		DWORD Thread::resume()
		{
			DWORD count = ResumeThread(m_handle);
			if (count == (DWORD)-1)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return count;
		}

		DWORD Thread::getId() const
		{
			DWORD id = GetThreadId(m_handle);
			if (id == 0)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return id;
		}

		DWORD Thread::getExitCode() const
		{
			DWORD exitCode = 0;
			if (!GetExitCodeThread(m_handle, &exitCode))
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return exitCode;
		}
	}
}

#include "Windows/Common.h"
#include "Windows/Thread.h"

#include "Windows/Util.h"

namespace win
{
	Thread::Thread() :
		m_thread(nullptr),
		m_id(INVALID_ID)
	{
	}

	Thread::Thread(DWORD id, DWORD desiredAccess, BOOL inheritHandle) :
		m_thread(nullptr),
		m_id(id)
	{
		m_thread = OpenThread(desiredAccess, inheritHandle, id);
		if (m_thread == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
	}

	Thread::Thread(Thread&& other) :
		m_thread(other.m_thread),
		m_id(other.m_id)
	{
		other.m_thread = nullptr;
		other.m_id = INVALID_ID;
	}

	Thread::~Thread()
	{
		if (m_thread != nullptr)
			CloseHandle(m_thread);
	}

	Thread& Thread::operator =(Thread&& other)
	{
		Thread(std::move(other)).swap(*this);
		return *this;
	}

	void Thread::swap(Thread& other)
	{
		std::swap(m_thread, other.m_thread);
		std::swap(m_id, other.m_id);
	}

	Thread::operator HANDLE() const
	{
		return m_thread;
	}

	DWORD Thread::getId() const
	{
		return m_id;
	}
}

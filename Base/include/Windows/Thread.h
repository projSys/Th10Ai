#pragma once

namespace win
{
	class Thread
	{
	public:
		static const DWORD INVALID_ID = 0xFFFFFFFF;

		Thread();
		explicit Thread(DWORD id, DWORD desiredAccess = THREAD_ALL_ACCESS,
			BOOL inheritHandle = FALSE);
		Thread(const Thread&) = delete;
		Thread(Thread&& other);
		~Thread();
		Thread& operator =(const Thread&) = delete;
		Thread& operator =(Thread&& other);
		void swap(Thread& other);

		operator HANDLE() const;
		DWORD getId() const;

	private:
		HANDLE m_thread;
		DWORD m_id;
	};
}

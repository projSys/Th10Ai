#include "Th10Hook/Common.h"
#include "Th10Hook/MyDetours.h"

#include <detours.h>

namespace th
{
	MyDetours::MyDetours() :
		m_commit(true)
	{
		transactionBegin();
	}

	MyDetours::~MyDetours()
	{
		try
		{
			if (m_commit)
				transactionCommit();
			else
				transactionAbort();
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}
	}

	void MyDetours::transactionBegin()
	{
		LONG errorId = DetourTransactionBegin();
		if (errorId != NO_ERROR)
			BASE_THROW(WindowsError(errorId));
	}

	void MyDetours::transactionCommit()
	{
		LONG errorId = DetourTransactionCommit();
		if (errorId != NO_ERROR)
			BASE_THROW(WindowsError(errorId));
	}

	void MyDetours::transactionAbort()
	{
		LONG errorId = DetourTransactionAbort();
		if (errorId != NO_ERROR)
			BASE_THROW(WindowsError(errorId));
	}

	void MyDetours::updateThread(HANDLE thread)
	{
		LONG errorId = DetourUpdateThread(thread);
		if (errorId != NO_ERROR)
		{
			m_commit = false;
			BASE_THROW(WindowsError(errorId));
		}
	}

	void MyDetours::attach(PVOID* pointer, PVOID detour)
	{
		LONG errorId = DetourAttach(pointer, detour);
		if (errorId != NO_ERROR)
		{
			m_commit = false;
			BASE_THROW(WindowsError(errorId));
		}
	}

	void MyDetours::detach(PVOID* pointer, PVOID detour)
	{
		LONG errorId = DetourDetach(pointer, detour);
		if (errorId != NO_ERROR)
		{
			m_commit = false;
			BASE_THROW(WindowsError(errorId));
		}
	}
}

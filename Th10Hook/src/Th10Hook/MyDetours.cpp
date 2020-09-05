#include "Th10Hook/MyDetours.h"

#include <detours.h>

namespace th
{
	void MyDetours::TransactionBegin()
	{
		LONG errorId = DetourTransactionBegin();
		if (errorId != NO_ERROR)
			BASE_THROW(WindowsError(errorId));
	}

	void MyDetours::TransactionCommit()
	{
		LONG errorId = DetourTransactionCommit();
		if (errorId != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorId));
		}
	}

	void MyDetours::UpdateThread(HANDLE thread)
	{
		LONG errorId = DetourUpdateThread(thread);
		if (errorId != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorId));
		}
	}

	void MyDetours::Attach(PVOID* pointer, PVOID detour)
	{
		LONG errorId = DetourAttach(pointer, detour);
		if (errorId != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorId));
		}
	}

	void MyDetours::Detach(PVOID* pointer, PVOID detour)
	{
		LONG errorId = DetourDetach(pointer, detour);
		if (errorId != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorId));
		}
	}
}

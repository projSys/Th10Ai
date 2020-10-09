#include "Th10Hook/MyDetours.h"

#include <detours.h>

namespace th
{
	void MyDetours::TransactionBegin()
	{
		LONG errorCode = DetourTransactionBegin();
		if (errorCode != NO_ERROR)
			BASE_THROW(WindowsError(errorCode));
	}

	void MyDetours::TransactionCommit()
	{
		LONG errorCode = DetourTransactionCommit();
		if (errorCode != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorCode));
		}
	}

	void MyDetours::UpdateThread(HANDLE thread)
	{
		LONG errorCode = DetourUpdateThread(thread);
		if (errorCode != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorCode));
		}
	}

	void MyDetours::Attach(PVOID* pointer, PVOID detour)
	{
		LONG errorCode = DetourAttach(pointer, detour);
		if (errorCode != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorCode));
		}
	}

	void MyDetours::Detach(PVOID* pointer, PVOID detour)
	{
		LONG errorCode = DetourDetach(pointer, detour);
		if (errorCode != NO_ERROR)
		{
			DetourTransactionAbort();
			BASE_THROW(WindowsError(errorCode));
		}
	}
}

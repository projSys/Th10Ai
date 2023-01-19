#include "Th10Hook/MyDetours.h"

#include <detours.h>
#include <Base/Windows/WindowsError.h>

namespace th
{
	void MyDetours::transactionBegin()
	{
		LONG ret = DetourTransactionBegin();
		if (ret != NO_ERROR)
			Throw(WindowsError(ret));
	}

	void MyDetours::transactionCommit()
	{
		LONG ret = DetourTransactionCommit();
		if (ret != NO_ERROR)
			Throw(WindowsError(ret));
	}

	void MyDetours::transactionAbort()
	{
		LONG ret = DetourTransactionAbort();
		//if (ret != NO_ERROR)
		//	Throw(WindowsError(ret));
	}

	void MyDetours::updateThread(HANDLE thread)
	{
		LONG ret = DetourUpdateThread(thread);
		if (ret != NO_ERROR)
			Throw(WindowsError(ret));
	}

	void MyDetours::attach(PVOID* pointer, PVOID detour)
	{
		LONG ret = DetourAttach(pointer, detour);
		if (ret != NO_ERROR)
			Throw(WindowsError(ret));
	}

	void MyDetours::detach(PVOID* pointer, PVOID detour)
	{
		LONG ret = DetourDetach(pointer, detour);
		if (ret != NO_ERROR)
			Throw(WindowsError(ret));
	}
}

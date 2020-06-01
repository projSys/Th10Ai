#include "libTh10Ai/Common.h"
#include "libTh10Ai/MyDetours.h"

#include <detours.h>

namespace th
{
	MyDetours::MyDetours()
	{
		DetourTransactionBegin();
	}

	MyDetours::~MyDetours()
	{
		DetourTransactionCommit();
	}

	void MyDetours::updateThread(HANDLE thread)
	{
		DetourUpdateThread(thread);
	}

	void MyDetours::attach(PVOID* pointer, PVOID detour)
	{
		DetourAttach(pointer, detour);
	}

	void MyDetours::detach(PVOID* pointer, PVOID detour)
	{
		DetourDetach(pointer, detour);
	}
}

#include "libTh10Ai/Common.h"
#include "libTh10Ai/libDetours.h"

#include <detours.h>

namespace th
{
	libDetours::libDetours()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
	}

	libDetours::~libDetours()
	{
		DetourTransactionCommit();
	}

	void libDetours::attach(PVOID* pointer, PVOID detour)
	{
		DetourAttach(pointer, detour);
	}

	void libDetours::detach(PVOID* pointer, PVOID detour)
	{
		DetourDetach(pointer, detour);
	}
}

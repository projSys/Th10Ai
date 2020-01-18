#include "libTh10Ai/Common.h"
#include "libTh10Ai/Util/Detour.h"

#include <detours.h>

namespace th
{
	Detour::Detour()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
	}

	Detour::~Detour()
	{
		DetourTransactionCommit();
	}

	void Detour::attach(PVOID* pointer, PVOID detour)
	{
		DetourAttach(pointer, detour);
	}

	void Detour::detach(PVOID* pointer, PVOID detour)
	{
		DetourDetach(pointer, detour);
	}
}

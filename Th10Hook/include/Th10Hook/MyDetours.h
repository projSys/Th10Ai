#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	// À±¼¦·â×°
	class MyDetours
	{
	public:
		static void TransactionBegin();
		static void TransactionAbort();
		static void TransactionCommit();

		// GetCurrentThread()
		static void UpdateThread(HANDLE thread);

		static void Attach(PVOID* pointer, PVOID detour);
		static void Detach(PVOID* pointer, PVOID detour);
	};
}

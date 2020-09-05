#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	class MyDetours
	{
	public:
		static void TransactionBegin();
		static void TransactionCommit();

		// GetCurrentThread()
		static void UpdateThread(HANDLE thread);

		static void Attach(PVOID* pointer, PVOID detour);
		static void Detach(PVOID* pointer, PVOID detour);
	};
}

#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	class MyDetours
	{
	public:
		void transactionBegin();
		void transactionAbort();
		void transactionCommit();

		void updateThread(HANDLE thread);

		void attach(PVOID* pointer, PVOID detour);
		void detach(PVOID* pointer, PVOID detour);
	};
}

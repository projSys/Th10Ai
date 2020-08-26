#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	class MyDetours
	{
	public:
		MyDetours();
		~MyDetours();

		// GetCurrentThread()
		void updateThread(HANDLE thread);

		void attach(PVOID* pointer, PVOID detour);
		void detach(PVOID* pointer, PVOID detour);

	private:
		void transactionBegin();
		void transactionCommit();
		void transactionAbort();

		bool m_commit;
	};
}

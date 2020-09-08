#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	class SelfFree
	{
	public:
		static void Free();

	private:
		static DWORD WINAPI FreeProc(LPVOID);
	};
}

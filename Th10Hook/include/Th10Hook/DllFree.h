#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	class DllFree
	{
	public:
		static void SelfFree();

	private:
		static DWORD WINAPI FreeProc(LPVOID);
	};
}

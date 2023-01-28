#include "Th10Hook/InlineHook32.h"

#include <Base/Exception/SystemError.h>

#pragma warning(disable: 6387)

namespace th
{
	void* InlineHook32::attach(void* original, void* detour)
	{
		HANDLE process = GetCurrentProcess();

		BYTE origCodes[5] = {};
		if (!ReadProcessMemory(process, original, origCodes, 5, nullptr))
			Throw(SystemError(GetLastError()));

		DWORD src = reinterpret_cast<DWORD>(original) + 5;
		DWORD dst = reinterpret_cast<DWORD>(detour);
		DWORD offset = dst - src;
		BYTE detCodes[5] = {};
		memcpy(detCodes, "\xE9", 1);
		memcpy(detCodes + 1, &offset, 4);
		if (!WriteProcessMemory(process, original, detCodes, 5, nullptr))
			Throw(SystemError(GetLastError()));

		BYTE* trampoline = reinterpret_cast<BYTE*>(
			VirtualAlloc(nullptr, 11, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
		if (trampoline == nullptr)
			Throw(SystemError(GetLastError()));
		memcpy(trampoline, origCodes, 5);
		memcpy(trampoline + 5, "\x68", 1);
		memcpy(trampoline + 6, &src, 4);
		memcpy(trampoline + 10, "\xC3", 1);
		return trampoline;
	}

	void InlineHook32::detach(PVOID* pointer, PVOID detour)
	{
	}
}

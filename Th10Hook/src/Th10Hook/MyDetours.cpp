#include "Th10Hook/MyDetours.h"

#include <system_error>
#include <detours.h>

namespace th
{
	void MyDetours::transactionBegin()
	{
		LONG ret = DetourTransactionBegin();
		if (ret != NO_ERROR)
			BASE_THROW(std::system_error(ret, std::system_category()));
	}

	void MyDetours::transactionCommit()
	{
		LONG ret = DetourTransactionCommit();
		if (ret != NO_ERROR)
			BASE_THROW(std::system_error(ret, std::system_category()));
	}

	void MyDetours::transactionAbort()
	{
		LONG ret = DetourTransactionAbort();
		//if (ret != NO_ERROR)
		//	BASE_THROW(std::system_error(ret, std::system_category()));
	}

	void MyDetours::updateThread(HANDLE thread)
	{
		LONG ret = DetourUpdateThread(thread);
		if (ret != NO_ERROR)
			BASE_THROW(std::system_error(ret, std::system_category()));
	}

	void MyDetours::attach(PVOID* pointer, PVOID detour)
	{
		LONG ret = DetourAttach(pointer, detour);
		if (ret != NO_ERROR)
			BASE_THROW(std::system_error(ret, std::system_category()));
	}

	void MyDetours::detach(PVOID* pointer, PVOID detour)
	{
		LONG ret = DetourDetach(pointer, detour);
		if (ret != NO_ERROR)
			BASE_THROW(std::system_error(ret, std::system_category()));
	}
}

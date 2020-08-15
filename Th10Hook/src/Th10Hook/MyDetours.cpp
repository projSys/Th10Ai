#include "Th10Hook/Common.h"
#include "Th10Hook/MyDetours.h"

#include <detours.h>

namespace th
{
	MyDetours::MyDetours() :
		m_commit(true)
	{
		transactionBegin();
	}

	MyDetours::~MyDetours()
	{
		try
		{
			if (m_commit)
				transactionCommit();
			else
				transactionAbort();
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}
	}

	void MyDetours::transactionBegin()
	{
		LONG errorId = DetourTransactionBegin();
		if (errorId != NO_ERROR)
			BASE_THROW(Exception(getErrorDesc(errorId + 10000)));
	}

	void MyDetours::transactionCommit()
	{
		LONG errorId = DetourTransactionCommit();
		if (errorId != NO_ERROR)
			BASE_THROW(Exception(getErrorDesc(errorId + 20000)));
	}

	void MyDetours::transactionAbort()
	{
		LONG errorId = DetourTransactionAbort();
		if (errorId != NO_ERROR)
			BASE_THROW(Exception(getErrorDesc(errorId + 30000)));
	}

	void MyDetours::updateThread(HANDLE thread)
	{
		LONG errorId = DetourUpdateThread(thread);
		if (errorId != NO_ERROR)
		{
			m_commit = false;
			BASE_THROW(Exception(getErrorDesc(errorId + 40000)));
		}
	}

	void MyDetours::attach(PVOID* pointer, PVOID detour)
	{
		LONG errorId = DetourAttach(pointer, detour);
		if (errorId != NO_ERROR)
		{
			m_commit = false;
			BASE_THROW(Exception(getErrorDesc(errorId + 50000)));
		}
	}

	void MyDetours::detach(PVOID* pointer, PVOID detour)
	{
		LONG errorId = DetourDetach(pointer, detour);
		if (errorId != NO_ERROR)
		{
			m_commit = false;
			BASE_THROW(Exception(getErrorDesc(errorId + 60000)));
		}
	}

	const char* MyDetours::getErrorDesc(LONG errorId)
	{
		switch (errorId)
		{
			// DetourTransactionBegin
		case ERROR_INVALID_OPERATION + 10000:
			return "ERROR_INVALID_OPERATION : A pending transaction alrady exists.";

			// DetourTransactionCommit
		case ERROR_INVALID_DATA + 20000:
			return "ERROR_INVALID_DATA : Target function was changed by third party between steps of the transaction.";

		case ERROR_INVALID_OPERATION + 20000:
			return "ERROR_INVALID_OPERATION : No pending transaction exists.";

			// DetourTransactionAbort
		case ERROR_INVALID_OPERATION + 30000:
			return "ERROR_INVALID_OPERATION : No pending transaction exists.";

			// DetourUpdateThread
		case ERROR_NOT_ENOUGH_MEMORY + 40000:
			return "ERROR_NOT_ENOUGH_MEMORY : Not enough memory to record identity of thread.";

			// DetourAttach
		case ERROR_INVALID_BLOCK + 50000:
			return "ERROR_INVALID_BLOCK : The function referenced is too small to be detoured.";

		case ERROR_INVALID_HANDLE + 50000:
			return "ERROR_INVALID_HANDLE : The ppPointer parameter is null or points to a null pointer.";

		case ERROR_INVALID_OPERATION + 50000:
			return "ERROR_INVALID_OPERATION : No pending transaction exists.";

		case ERROR_NOT_ENOUGH_MEMORY + 50000:
			return "ERROR_NOT_ENOUGH_MEMORY : Not enough memory exists to complete the operation.";

			// DetourDetach
		case ERROR_INVALID_BLOCK + 60000:
			return "ERROR_INVALID_BLOCK : The function to be detached was too small to be detoured.";

		case ERROR_INVALID_HANDLE + 60000:
			return "ERROR_INVALID_HANDLE : The ppPointer parameter is null or references a null address.";

		case ERROR_INVALID_OPERATION + 60000:
			return "ERROR_INVALID_OPERATION : No pending transaction exists.";

		case ERROR_NOT_ENOUGH_MEMORY + 60000:
			return "ERROR_NOT_ENOUGH_MEMORY : Not enough memory to complete the operation.";

		default:
			return "Unknown error.";
		}
	}
}

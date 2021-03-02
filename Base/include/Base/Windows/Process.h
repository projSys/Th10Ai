#pragma once

#include "Base/Windows/Common.h"

#include "Base/Windows/Handle.h"
#include "Base/Windows/Thread.h"

namespace base
{
	namespace win
	{
		class Process :
			public Handle
		{
		public:
			//static Process Create();
			static Process Open(DWORD desiredAccess, BOOL inheritHandle, DWORD processId);

			using Handle::Handle;

			Thread createRemoteThread(LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize,
				LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD creationFlags);

			DWORD getId() const;
			DWORD getExitCode() const;
		};
	}
}

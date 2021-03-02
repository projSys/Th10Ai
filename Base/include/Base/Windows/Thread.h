#pragma once

#include "Base/Windows/Common.h"

#include "Base/Windows/Handle.h"

namespace base
{
	namespace win
	{
		class Thread :
			public Handle
		{
		public:
			static Thread Create(LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize,
				LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD creationFlags);
			static Thread Open(DWORD desiredAccess, BOOL inheritHandle, DWORD threadId);

			using Handle::Handle;

			DWORD getId() const;
			DWORD getExitCode() const;
		};
	}
}

#pragma once

#include "Base/Windows/Common.h"

#include <utility>
#include <boost/filesystem.hpp>

#include "Base/Windows/Handle.h"

namespace base
{
	namespace win
	{
		namespace fs = boost::filesystem;

		class Thread;

		class Process :
			public Handle
		{
		public:
			static std::pair<Process, Thread> Create(const fs::path& applicationName,
				LPWSTR commandLine, LPSECURITY_ATTRIBUTES processAttributes,
				LPSECURITY_ATTRIBUTES threadAttributes, BOOL inheritHandles,
				DWORD creationFlags, LPVOID environment, const fs::path& currentDirectory,
				LPSTARTUPINFOW startupInfo);
			static Process Open(DWORD desiredAccess, BOOL inheritHandle, DWORD processId);

			using Handle::Handle;

			Thread createRemoteThread(LPSECURITY_ATTRIBUTES threadAttributes, SIZE_T stackSize,
				LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD creationFlags);

			DWORD getId() const;
			DWORD getExitCode() const;
		};
	}
}

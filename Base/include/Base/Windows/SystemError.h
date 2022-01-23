#pragma once

#include "Base/Common.h"

#include <system_error>

#include "Base/Throwable.h"

namespace base
{
	class SystemError :
		public std::system_error,
		public Throwable
	{
	public:
		SystemError(DWORD errorCode = GetLastError(),
			const std::source_location& loc = std::source_location::current());

		virtual void toStream(std::ostream& os) const override;
	};
}

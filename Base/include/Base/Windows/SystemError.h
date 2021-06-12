#pragma once

#include "Base/Common.h"

#include <system_error>

#include "Base/Exception.h"

namespace base
{
	class SystemError :
		public std::system_error,
		public Throwable
	{
	public:
		SystemError(DWORD errorCode,
			const boost::source_location& loc);

		virtual void print(std::ostream& os) const override;
	};
}

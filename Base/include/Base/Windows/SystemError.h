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
		SystemError(DWORD errorCode,
			const boost::source_location& loc);

		virtual void toStream(std::ostream& os) const override;
	};
}

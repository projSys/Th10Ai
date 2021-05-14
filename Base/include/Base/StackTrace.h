#pragma once

#include "Base/Common.h"

#include <ostream>

#include "Base/Types.h"
#ifdef BASE_WINDOWS
#include "Base/Windows/StackTraceImpl.h"
#endif

namespace base
{
	class StackTrace
	{
	public:
		StackTrace();
		explicit StackTrace(uint_t skipped);

		void print(std::ostream& os) const;

	private:
#ifdef BASE_WINDOWS
		win::StackTraceImpl m_impl;
#endif
	};
}

#pragma once

#include "Base/Common.h"

#include "Base/Types.h"
#include "Base/Streamable.h"
#ifdef BASE_WINDOWS
#include "Base/Windows/StackTraceImpl.h"
#endif

namespace base
{
	class StackTrace :
		public OStreamable
	{
	public:
		explicit StackTrace(uint_t skips = 2);

		virtual void toStream(std::ostream& os) const override;

	private:
#ifdef BASE_WINDOWS
		win::StackTraceImpl m_impl;
#endif
	};
}

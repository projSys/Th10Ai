#include "Base/StackTrace.h"

namespace base
{
	StackTrace::StackTrace(uint_t skips)
#ifdef BASE_WINDOWS
		: m_impl(skips)
#endif
	{
	}

	void StackTrace::toStream(std::ostream& os) const
	{
#ifdef BASE_WINDOWS
		m_impl.toStream(os);
#endif
	}
}

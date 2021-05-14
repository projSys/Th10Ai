#include "Base/StackTrace.h"

namespace base
{
	StackTrace::StackTrace()
	{
	}

	StackTrace::StackTrace(uint_t skipped) :
		m_impl(skipped)
	{
	}

	void StackTrace::print(std::ostream& os) const
	{
		m_impl.print(os);
	}
}

#include "Base/SourceLocation.h"

namespace base
{
	SourceLocation::SourceLocation(
		const char* func, const char* file, uint_t line) :
		ThreadLocal(this),
		m_func(func),
		m_file(file),
		m_line(line)
	{
	}

	void SourceLocation::printTo(std::ostream& os) const
	{
		os << " in " << m_func
			<< " at " << m_file
			<< " : " << m_line << '\n';
	}
}

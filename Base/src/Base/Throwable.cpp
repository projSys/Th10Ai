#include "Base/Throwable.h"

#include <ostream>

namespace base
{
	Throwable::Throwable(const std::source_location& loc) :
		m_loc(loc)
	{
	}

	void Throwable::toStream(std::ostream& os) const
	{
		os << "SourceLocation:\n"
			<< " in " << m_loc.function_name()
			<< " at " << m_loc.file_name()
			<< " : " << m_loc.line()
			<< '\n';
	}
}

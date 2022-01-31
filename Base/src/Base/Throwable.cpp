#include "Base/Throwable.h"

#include <ostream>

namespace base
{
	Throwable::Throwable(const std::source_location& sl) :
		m_sl(sl)
	{
	}

	void Throwable::toStream(std::ostream& os) const
	{
		os << "SourceLocation:\n"
			<< " in " << m_sl.function_name()
			<< " at " << m_sl.file_name()
			<< " : " << m_sl.line()
			<< '\n';
	}
}

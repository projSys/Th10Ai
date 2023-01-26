#include "Base/Throw.h"

#include <ostream>

namespace base
{
	std::ostream& operator <<(std::ostream& os, const std::source_location& sl)
	{
		os << "SourceLocation:\n"
			<< " in " << sl.function_name()
			//TODO：ANSI转UTF8
			<< " at " << sl.file_name()
			<< " : " << sl.line()
			<< '\n';
		return os;
	}

	const std::source_location Throwable::s_invalidSl;

	void Throwable::toStream(std::ostream& os) const
	{
	}

	const std::source_location& Throwable::getSourceLocation() const
	{
		return s_invalidSl;
	}
}

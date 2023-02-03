#include "Base/Exception/Throw.h"

#include <ostream>

namespace base
{
	std::ostream& operator <<(std::ostream& os, const std::source_location& sl)
	{
		os << "SourceLocation:\n"
			<< " in " << sl.function_name()
			// 不需要转码，和代码文件的编码一致
			<< " at " << sl.file_name()
			<< ':' << sl.line()
			<< '\n';
		return os;
	}

	const std::source_location Throwable::s_invalidSl;
	const StackTrace Throwable::s_invalidSt(0, 0);

	void Throwable::toStream(std::ostream& os) const
	{
	}

	const std::source_location& Throwable::getSourceLocation() const
	{
		return s_invalidSl;
	}

	const StackTrace& Throwable::getStackTrace() const
	{
		return s_invalidSt;
	}
}

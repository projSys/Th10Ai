#include "Base/Exception.h"

#include <ostream>

namespace base
{
	Exception::Exception(const char* str,
		const std::source_location& sl,
		const StackTrace& st) :
		runtime_error(str),
		Throwable(sl, st)
	{
	}

	Exception::Exception(const std::string& str,
		const std::source_location& sl,
		const StackTrace& st) :
		runtime_error(str),
		Throwable(sl, st)
	{
	}

	void Exception::toStream(std::ostream& os) const
	{
		os << what() << '\n';
		Throwable::toStream(os);
	}
}

#include "Base/Exception.h"

#include <ostream>

namespace base
{
	Exception::Exception(const char* str,
		const std::source_location& loc) :
		runtime_error(str),
		Throwable(loc)
	{
	}

	Exception::Exception(const std::string& str,
		const std::source_location& loc) :
		runtime_error(str),
		Throwable(loc)
	{
	}

	void Exception::toStream(std::ostream& os) const
	{
		os << what() << '\n';
		Throwable::toStream(os);
	}
}

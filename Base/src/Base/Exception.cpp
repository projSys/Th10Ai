#include "Base/Exception.h"

#include <ostream>

namespace base
{
	Exception::Exception(const char* str,
		const std::source_location& sl) :
		runtime_error(str),
		Throwable(sl)
	{
	}

	Exception::Exception(const std::string& str,
		const std::source_location& sl) :
		runtime_error(str),
		Throwable(sl)
	{
	}

	void Exception::toStream(std::ostream& os) const
	{
		os << what() << '\n';
		Throwable::toStream(os);
	}
}

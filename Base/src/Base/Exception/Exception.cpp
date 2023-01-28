#include "Base/Exception/Exception.h"

#include <ostream>

namespace base
{
	Exception::Exception(const char* str) :
		runtime_error(str)
	{
	}

	Exception::Exception(const std::string& str) :
		runtime_error(str)
	{
	}

	void Exception::toStream(std::ostream& os) const
	{
		os << what() << '\n';
	}
}

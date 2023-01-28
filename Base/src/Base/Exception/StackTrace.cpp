#include "Base/Exception/StackTrace.h"

#include <ostream>

namespace base
{
	std::ostream& operator <<(std::ostream& os, const StackTrace& st)
	{
#ifdef BASE_THROW_STACKTRACE
		os << "StackTrace:\n";
		bst::operator <<(os, st);
#endif
		return os;
	}
}

#include "Base/Exception/StackTrace.h"

#include <ostream>
#ifdef BASE_THROW_STACKTRACE
#include <boost/locale.hpp>
#endif

namespace base
{
#ifdef BASE_THROW_STACKTRACE
	namespace bloc = boost::locale;
	namespace blc = boost::locale::conv;
	namespace blu = boost::locale::util;
#endif

	std::ostream& operator <<(std::ostream& os, const StackTrace& st)
	{
#ifdef BASE_THROW_STACKTRACE
		os << "StackTrace:\n";
		std::string buf = bst::to_string(st);
		std::string id = blu::get_system_locale();
		if (id.find("UTF-8") == std::string::npos)
		{
			bloc::generator gen;
			buf = blc::to_utf<char>(buf, gen(id));
		}
		os << buf;
#endif
		return os;
	}
}

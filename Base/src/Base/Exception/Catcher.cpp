#include "Base/Exception/Catcher.h"

#include <ostream>
#include <exception>
#include <boost/exception/diagnostic_information.hpp>

#include "Base/Exception/Throw.h"

namespace base
{
	void Catcher::toStream(std::ostream& os) const
	{
		try
		{
			throw;
		}
		catch (const Throwable& throwable)
		{
			os << throwable;
		}
		catch (const boost::exception& be)
		{
			os << boost::diagnostic_information(be);
		}
		catch (const std::exception& se)
		{
			os << se.what() << '\n';
		}
	}
}

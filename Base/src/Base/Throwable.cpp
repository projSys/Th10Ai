#include "Base/Throwable.h"

#include <ostream>
#include <sstream>
#include <exception>
#include <boost/exception/diagnostic_information.hpp>

namespace base
{
	Throwable::Throwable(const boost::source_location& loc) :
		m_loc(loc)
	{
	}

	void Throwable::toStream(std::ostream& os) const
	{
		os << "SourceLocation:\n"
			<< " in " << m_loc.function_name()
			<< " at " << m_loc.file_name()
			<< " : " << m_loc.line()
			<< '\n';
	}

	std::string PrintException()
	{
		std::ostringstream oss;
		try
		{
			throw;
		}
		catch (const Throwable& throwable)
		{
			oss << throwable;
		}
		catch (const boost::exception& be)
		{
			oss << boost::diagnostic_information(be);
		}
		catch (const std::exception& se)
		{
			oss << se.what() << '\n';
		}
		catch (...)
		{
			oss << "Unclassified exception.\n";
		}
		return oss.str();
	}
}

#include "Base/Exception.h"

#include <sstream>
#include <boost/exception/diagnostic_information.hpp>

namespace base
{
	Throwable::Throwable(const boost::source_location& loc) :
		m_loc(loc)
	{
	}

	void Throwable::print(std::ostream& os) const
	{
		os << "SourceLocation:\n"
			<< " in " << m_loc.function_name()
			<< " at " << m_loc.file_name()
			<< " : " << m_loc.line()
			<< '\n';
	}

	Exception::Exception(const char* str,
		const boost::source_location& loc) :
		runtime_error(str),
		Throwable(loc)
	{
	}

	Exception::Exception(const std::string& str,
		const boost::source_location& loc) :
		runtime_error(str),
		Throwable(loc)
	{
	}

	void Exception::print(std::ostream& os) const
	{
		os << what() << '\n';
		Throwable::print(os);
	}

	std::string PrintException()
	{
		std::ostringstream oss;
		try
		{
			throw;
		}
		catch (const Throwable& ex)
		{
			ex.print(oss);
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

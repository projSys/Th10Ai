#include "Base/Exception.h"

#include <sstream>
#include <boost/exception/all.hpp>

namespace base
{
	Exception::Exception(const char* str) :
		runtime_error(str),
		m_sourceLocation(SourceLocation::GetInstance())
#ifdef BASE_WINDOWS
		, m_stackTrace(win::StackTrace::GetInstance())
#endif
	{
	}

	Exception::Exception(const std::string& str) :
		runtime_error(str),
		m_sourceLocation(SourceLocation::GetInstance())
#ifdef BASE_WINDOWS
		, m_stackTrace(win::StackTrace::GetInstance())
#endif
	{
	}

	void Exception::print(std::ostream& os) const
	{
		os << what() << '\n';

		m_sourceLocation.print(os);
#ifdef BASE_WINDOWS
		m_stackTrace.print(os);
#endif
	}

	std::string PrintException()
	{
		std::ostringstream oss;
		try
		{
			throw;
		}
		catch (const Exception& ex)
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
			oss << "Unknown exception.\n";
		}
		return oss.str();
	}
}

#include "Base/Exception.h"

#include <sstream>
#include <boost/exception/all.hpp>

namespace base
{
	Exception::Exception(uint_t framesToSkip) :
		runtime_error(nullptr),
		m_sourceLocation(SourceLocation::GetCurrent()),
		m_stackTrace(framesToSkip + 1)
	{
	}

	Exception::Exception(const char* str, uint_t framesToSkip) :
		runtime_error(str),
		m_sourceLocation(SourceLocation::GetCurrent()),
		m_stackTrace(framesToSkip + 1)
	{
	}

	Exception::Exception(const std::string& str, uint_t framesToSkip) :
		runtime_error(str),
		m_sourceLocation(SourceLocation::GetCurrent()),
		m_stackTrace(framesToSkip + 1)
	{
	}

	void Exception::printTo(std::ostream& os) const
	{
		os << what() << '\n';

		m_sourceLocation.printTo(os);
		m_stackTrace.printTo(os);
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
			ex.printTo(oss);
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

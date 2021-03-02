#include "Base/Exception.h"

#include <sstream>
#include <boost/exception/all.hpp>

namespace base
{
	Exception::Exception(const char* str, uint_t framesToSkip) :
		runtime_error(str),
		m_sourceLocation(SourceLocation::Get()),
		m_stackTrace(framesToSkip + 1)
	{
	}

	Exception::Exception(const std::string& str, uint_t framesToSkip) :
		runtime_error(str),
		m_sourceLocation(SourceLocation::Get()),
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
		std::ostringstream sout;
		try
		{
			throw;
		}
		catch (const Exception& ex)
		{
			ex.printTo(sout);
		}
		catch (const boost::exception& be)
		{
			sout << boost::diagnostic_information(be);
		}
		catch (const std::exception& se)
		{
			sout << se.what() << '\n';
		}
		catch (...)
		{
			sout << "Unclassified exception.\n";
		}
		return sout.str();
	}
}

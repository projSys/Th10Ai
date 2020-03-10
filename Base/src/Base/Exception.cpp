#include "Base/Common.h"
#include "Base/Exception.h"

#include <boost/exception/all.hpp>

namespace base
{
	Exception::Exception(const std::string& whatArg) :
		std::runtime_error(whatArg),
		m_sourceLocation(SourceLocation::GetCurrent())
#ifdef _DEBUG
		, m_stackTrace(3, std::numeric_limits<uint_t>::max() - 3)
#endif
	{
	}

	Exception::Exception(const char* whatArg) :
		std::runtime_error(whatArg),
		m_sourceLocation(SourceLocation::GetCurrent())
#ifdef _DEBUG
		, m_stackTrace(3, std::numeric_limits<uint_t>::max() - 3)
#endif
	{
	}

	void Exception::print(std::ostream& os) const
	{
		os << what() << '\n';
		printSourceLocation(os);
		printStackTrace(os);
	}

	void Exception::printSourceLocation(std::ostream& os) const
	{
		os << " in " << m_sourceLocation.getFunc()
			<< " at " << m_sourceLocation.getFile()
			<< ':' << m_sourceLocation.getLine() << '\n';
	}

	void Exception::printStackTrace(std::ostream& os) const
	{
#ifdef _DEBUG
		os << "StackTrace:\n" << m_stackTrace;
#endif
	}

	void PrintException(std::ostream& os)
	{
		try
		{
			throw;
		}
		catch (const Exception& ex)
		{
			ex.print(os);
		}
		catch (const boost::exception& be)
		{
			os << boost::diagnostic_information(be);
		}
		catch (const std::exception& se)
		{
			os << se.what() << '\n';
		}
		catch (...)
		{
			os << "Unclassified exception.\n";
		}
	}
}

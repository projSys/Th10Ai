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

	void PrintAllException(std::ostream& os)
	{
		try
		{
			throw;
		}
		catch (const Exception& ex)
		{
			ex.print(os);
		}
		catch (const boost::exception& bex)
		{
			os << boost::diagnostic_information(bex);
		}
		catch (const std::exception& sex)
		{
			os << sex.what();
		}
		catch (...)
		{
			os << "Unclassified exception.";
		}
	}
}

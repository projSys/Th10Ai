#include "Base/Common.h"
#include "Base/Exception.h"

#include <boost/exception/all.hpp>

namespace base
{
	Exception::Exception() :
		runtime_error(nullptr),
		m_id(0),
		m_sourceLocation(SourceLocation::GetCurrent())
	{
	}

	Exception::Exception(const string& desc) :
		runtime_error(desc),
		m_id(0),
		m_sourceLocation(SourceLocation::GetCurrent())
	{
	}

	Exception::Exception(const char* desc) :
		runtime_error(desc),
		m_id(0),
		m_sourceLocation(SourceLocation::GetCurrent())
	{
	}

	Exception::Exception(int_t id, const string& desc) :
		runtime_error(desc),
		m_id(id),
		m_sourceLocation(SourceLocation::GetCurrent())
	{
	}

	Exception::Exception(int_t id, const char* desc) :
		runtime_error(desc),
		m_id(id),
		m_sourceLocation(SourceLocation::GetCurrent())
	{
	}

	void Exception::print(ostream& os) const
	{
		os << '[' << m_id << ']' << what() << '\n';

		printSourceLocation(os);
		printStackTrace(os);
	}

	void Exception::printSourceLocation(ostream& os) const
	{
		os << " in " << m_sourceLocation.getFunc()
			<< " at " << m_sourceLocation.getFile()
			<< ':' << m_sourceLocation.getLine() << '\n';
	}

	void Exception::printStackTrace(ostream& os) const
	{
#ifdef _DEBUG
		os << "StackTrace:\n" << m_stackTrace;
#endif
	}

	void PrintException(ostream& os)
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
		catch (const exception& se)
		{
			os << se.what() << '\n';
		}
		catch (...)
		{
			os << "Unclassified exception.\n";
		}
	}
}

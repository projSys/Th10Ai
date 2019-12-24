#include "Base/Common.h"
#include "Base/Exception.h"

#include <boost/exception/all.hpp>

namespace base
{
	Exception::Exception(const std::string& whatArg) :
		std::runtime_error(whatArg)
	{
	}

	Exception::Exception(const char* whatArg) :
		std::runtime_error(whatArg)
	{
	}

	void Exception::print(std::ostream& os) const
	{
		os << what() << '\n';
	}

	ExceptionExtra::ExceptionExtra(const char* func, const char* file, uint_t line) :
		m_func(func),
		m_file(file),
		m_line(line)
#ifdef _DEBUG
		, m_stackTrace(5, std::numeric_limits<uint_t>::max() - 5)
#endif
	{
	}

	void ExceptionExtra::print(std::ostream& os) const
	{
		os << " in " << m_func << " at " << m_file << ':' << m_line << '\n';
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
		catch (const boost::exception& be)
		{
			os << boost::diagnostic_information(be);
		}
		catch (const std::exception& se)
		{
			os << se.what();
		}
		catch (...)
		{
			os << "Unclassified exception.";
		}
	}
}

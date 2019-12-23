#include "Base/Common.h"
#include "Base/Exception.h"

#include <boost/exception/all.hpp>

namespace base
{
	Exception::Exception(const std::string& whatArg,
		const char* func, const char* file, uint_t line
#ifdef _DEBUG
		, uint_t skip) :
#else
		, uint_t) :
#endif
	std::runtime_error(whatArg),
		m_func(func),
		m_file(file),
		m_line(line)
#ifdef _DEBUG
		, m_stackTrace(skip, std::numeric_limits<uint_t>::max() - skip)
#endif
	{
	}

	Exception::Exception(const char* whatArg,
		const char* func, const char* file, uint_t line
#ifdef _DEBUG
		, uint_t skip) :
#else
		, uint_t) :
#endif
	std::runtime_error(whatArg),
		m_func(func),
		m_file(file),
		m_line(line)
#ifdef _DEBUG
		, m_stackTrace(skip, std::numeric_limits<uint_t>::max() - skip)
#endif
	{
	}

	void Exception::print(std::ostream& os) const
	{
		os << what() << '\n';
		os << " in " << m_func << " at " << m_file << ':' << m_line << '\n';
#ifdef _DEBUG
		os << "StackTrace:\n" << m_stackTrace;
#endif
	}

	const char* Exception::getFunc() const
	{
		return m_func;
	}

	const char* Exception::getFile() const
	{
		return m_file;
	}

	uint_t Exception::getLine() const
	{
		return m_line;
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

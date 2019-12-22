#include "Base/Common.h"
#include "Base/Exception.h"

namespace base
{
	Exception::Exception(const std::string& whatArg,
		const char* func, const char* file, uint_t line) :
		std::runtime_error(whatArg),
		m_func(func),
		m_file(file),
		m_line(line)
	{
	}

	Exception::Exception(const char* whatArg,
		const char* func, const char* file, uint_t line) :
		std::runtime_error(whatArg),
		m_func(func),
		m_file(file),
		m_line(line)
	{
	}

	void Exception::print(std::ostream& os) const
	{
		os << what() << std::endl;
		os << " in " << m_func << std::endl;
		os << " at " << m_file << ":" << m_line << std::endl;
#ifdef _DEBUG
		os << "StackTrace:" << std::endl;
		os << m_stackTrace << std::endl;
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
}

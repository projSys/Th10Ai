#include "Base/SourceLocation.h"

namespace base
{
	SourceLocation::SourceLocation() :
		m_func(""),
		m_file(""),
		m_line(0)
	{
	}

	SourceLocation::SourceLocation(
		const char* func, const char* file, uint_t line) :
		m_func(func),
		m_file(file),
		m_line(line)
	{
	}

	void SourceLocation::print(std::ostream& os) const
	{
		os << "SourceLocation:\n"
			<< " in " << m_func
			<< " at " << m_file
			<< " : " << m_line << '\n';
	}
}

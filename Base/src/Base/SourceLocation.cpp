#include "Base/SourceLocation.h"

namespace base
{
	thread_local SourceLocation SourceLocation::stl_current;

	void SourceLocation::Set(const char* func, const char* file, uint_t line)
	{
		stl_current.m_func = func;
		stl_current.m_file = file;
		stl_current.m_line = line;
	}

	const SourceLocation& SourceLocation::Get()
	{
		return stl_current;
	}

	SourceLocation::SourceLocation() :
		m_func(nullptr),
		m_file(nullptr),
		m_line(0)
	{
	}

	void SourceLocation::printTo(std::ostream& os) const
	{
		os << " in " << m_func
			<< " at " << m_file
			<< " : " << m_line << '\n';
	}

	const char* SourceLocation::getFunc() const
	{
		return m_func;
	}

	const char* SourceLocation::getFile() const
	{
		return m_file;
	}

	uint_t SourceLocation::getLine() const
	{
		return m_line;
	}
}

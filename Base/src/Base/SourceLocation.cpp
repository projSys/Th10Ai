#include "Base/Common.h"
#include "Base/SourceLocation.h"

namespace base
{
	thread_local SourceLocation SourceLocation::tls_current;

	void SourceLocation::SetCurrent(
		const char* func, const char* file, uint_t line)
	{
		tls_current.m_func = func;
		tls_current.m_file = file;
		tls_current.m_line = line;
	}

	const SourceLocation& SourceLocation::GetCurrent()
	{
		return tls_current;
	}

	SourceLocation::SourceLocation() :
		m_func(nullptr),
		m_file(nullptr),
		m_line(0)
	{
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

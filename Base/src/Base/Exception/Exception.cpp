#include "Base/Exception/Exception.h"

#include <ostream>

namespace base
{
	Exception::Exception(const char* str) :
		m_impl(str)
	{
	}

	Exception::Exception(const std::string& str) :
		m_impl(str)
	{
	}

	void Exception::toStream(std::ostream& os) const
	{
		os << m_impl.what() << '\n';
	}

	const char* Exception::what() const
	{
		return m_impl.what();
	}
}

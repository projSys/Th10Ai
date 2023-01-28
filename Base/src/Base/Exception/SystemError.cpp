#include "Base/Exception/SystemError.h"

#include <ostream>

namespace base
{
	SystemError::SystemError(int32_t value,
		const std::error_category& category) :
		m_ec(value, category)
	{
	}

	SystemError::SystemError(const std::error_code& ec) :
		m_ec(ec)
	{
	}

	void SystemError::toStream(std::ostream& os) const
	{
		//TODO：ANSI转UTF8
		std::string msg = m_ec.message();
		os << msg.c_str() << '\n';
	}

	int32_t SystemError::getValue() const
	{
		return m_ec.value();
	}

	const std::error_code& SystemError::getErrorCode() const
	{
		return m_ec;
	}
}

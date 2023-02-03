#include "Base/Exception/SystemError.h"

#include <ostream>
#include <boost/locale.hpp>

namespace base
{
	namespace bloc = boost::locale;
	namespace blc = boost::locale::conv;
	namespace blu = boost::locale::util;

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
		std::string msg = m_ec.message();
		std::string id = blu::get_system_locale();
		if (id.find("UTF-8") == std::string::npos)
		{
			bloc::generator gen;
			msg = blc::to_utf<char>(msg, gen(id));
		}
		os << msg << '\n';
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

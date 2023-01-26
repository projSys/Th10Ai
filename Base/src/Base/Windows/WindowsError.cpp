#include "Base/Windows/WindowsError.h"

#include <ostream>

namespace base
{
	namespace win
	{
		WindowsError::WindowsError(const std::error_code& ec) :
			m_ec(ec)
		{
		}

		WindowsError::WindowsError(DWORD value,
			const std::error_category& category) :
			m_ec(value, category)
		{
		}

		void WindowsError::toStream(std::ostream& os) const
		{
			//TODO：ANSI转UTF8
			std::string msg = m_ec.message();
			os << msg.c_str() << '\n';
		}

		const std::error_code& WindowsError::getErrorCode() const
		{
			return m_ec;
		}
	}
}

#include "Base/Windows/WindowsError.h"

#include <ostream>

namespace base
{
	namespace win
	{
		WindowsError::WindowsError(DWORD value) :
			Exception(nullptr),
			m_ec(value, std::system_category())
		{
		}

		void WindowsError::toStream(std::ostream& os) const
		{
			//TODO：ANSI转UTF8
			std::string msg = m_ec.message();
			os << msg.c_str() << '\n';
		}
	}
}

#include "Windows/Common.h"
#include "Windows/WindowsError.h"

#include "Windows/Util.h"

namespace win
{
	WindowsError::WindowsError(DWORD error) :
		Exception(Util::GetErrorMessage(error)),
		m_error(error)
	{
	}

	void WindowsError::print(std::ostream& os) const
	{
		os << '[' << m_error << ']';
		Exception::print(os);
	}
}

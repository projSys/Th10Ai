#include "Windows/Common.h"
#include "Windows/WindowsError.h"

#include "Windows/Util.h"

namespace win
{
	WindowsError::WindowsError(DWORD error,
		const char* func, const char* file, uint_t line) :
		Exception(Util::GetErrorMessage(error), func, file, line, 4),
		m_error(error)
	{
	}

	void WindowsError::print(std::ostream& os) const
	{
		os << '[' << m_error << ']';
		Exception::print(os);
	}

	DWORD WindowsError::getError() const
	{
		return m_error;
	}
}

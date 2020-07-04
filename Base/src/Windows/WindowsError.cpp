#include "Windows/Common.h"
#include "Windows/WindowsError.h"

#include "Windows/Utils.h"

namespace win
{
	WindowsError::WindowsError(DWORD errorId) :
		m_errorId(errorId)
	{
	}

	void WindowsError::print(ostream& os) const
	{
		os << '[' << m_errorId << ']' << Utils::GetErrorDescription(m_errorId) << '\n';

		printSourceLocation(os);
		printStackTrace(os);
	}
}

#include "Windows/Common.h"
#include "Windows/WindowsError.h"

#include "Windows/Apis.h"

namespace win
{
	WindowsError::WindowsError(DWORD errorId) :
		m_errorId(errorId)
	{
	}

	void WindowsError::print(ostream& os) const
	{
		os << '[' << m_errorId << ']' << Apis::GetErrorDesc(m_errorId) << '\n';

		printSourceLocation(os);
		printStackTrace(os);
	}
}

#include "Windows/Common.h"
#include "Windows/WindowsError.h"

#include "Windows/Util.h"

namespace win
{
	WindowsError::WindowsError(DWORD errorId) :
		Exception(nullptr),
		m_errorId(errorId)
	{
	}

	void WindowsError::print(std::ostream& os) const
	{
		os << '[' << m_errorId << ']'
			<< Util::GetErrorDescription(m_errorId) << '\n';
		printSourceLocation(os);
		printStackTrace(os);
	}
}

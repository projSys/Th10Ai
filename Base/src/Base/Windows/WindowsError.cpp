#include "Base/Windows/WindowsError.h"

#include "Base/Windows/Apis.h"

namespace base
{
	namespace win
	{
		WindowsError::WindowsError(DWORD errorId) :
			Exception(1),
			m_errorId(errorId)
		{
		}

		void WindowsError::printTo(std::ostream& os) const
		{
			os << '[' << m_errorId << ']' << Apis::GetErrorDesc(m_errorId);

			m_sourceLocation.printTo(os);
			m_stackTrace.printTo(os);
		}
	}
}

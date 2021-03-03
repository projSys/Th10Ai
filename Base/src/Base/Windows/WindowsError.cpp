#include "Base/Windows/WindowsError.h"

#include "Base/Windows/Apis.h"

namespace base
{
	namespace win
	{
		WindowsError::WindowsError(DWORD errorCode) :
			Exception(nullptr),
			m_errorCode(errorCode)
		{
		}

		void WindowsError::printTo(std::ostream& os) const
		{
			os << '[' << m_errorCode << ']' << Apis::GetErrorDesc(m_errorCode);

			m_sourceLocation.printTo(os);
			m_stackTrace.printTo(os);
		}
	}
}

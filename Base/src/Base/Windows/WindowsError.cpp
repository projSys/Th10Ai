#include "Base/Windows/WindowsError.h"

#include "Base/Windows/Apis.h"

namespace base
{
	namespace win
	{
		WindowsError::WindowsError() :
			Exception(nullptr),
			m_errorCode(GetLastError())
		{
		}

		WindowsError::WindowsError(DWORD errorCode) :
			Exception(nullptr),
			m_errorCode(errorCode)
		{
		}

		void WindowsError::print(std::ostream& os) const
		{
			os << '[' << m_errorCode << ']'
				<< Apis::GetErrorDesc(m_errorCode).c_str() << '\n';

			stl_sourceLocation.print(os);
			stl_stackTrace.print(os);
		}
	}
}

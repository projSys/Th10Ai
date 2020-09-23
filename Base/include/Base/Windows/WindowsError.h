#pragma once

#include "Base/Windows/Common.h"

#include "Base/Exception.h"

namespace base
{
	namespace win
	{
		class WindowsError :
			public Exception
		{
		public:
			explicit WindowsError(DWORD errorId = GetLastError());

			virtual void printTo(std::ostream& os) const override;

		private:
			DWORD m_errorId;
		};
	}
}

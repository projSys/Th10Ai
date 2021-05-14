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
			WindowsError();
			explicit WindowsError(DWORD errorCode);

			virtual void print(std::ostream& os) const override;

		private:
			DWORD m_errorCode;
		};
	}
}

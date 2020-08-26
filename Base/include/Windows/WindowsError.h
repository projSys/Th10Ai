#pragma once

#include "Windows/Common.h"

#include "Base/Exception.h"

namespace win
{
	class WindowsError :
		public Exception
	{
	public:
		explicit WindowsError(DWORD errorId = GetLastError());

		virtual void print(std::ostream& os) const override;

	private:
		DWORD m_errorId;
	};
}

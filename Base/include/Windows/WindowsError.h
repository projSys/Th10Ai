#pragma once

#include "Base/Exception.h"

namespace win
{
	class WindowsError :
		public Exception
	{
	public:
		WindowsError(DWORD errorId);

		virtual void print(std::ostream& os) const override;

	private:
		DWORD m_errorId;
	};
}

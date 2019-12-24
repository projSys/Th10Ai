#pragma once

#include "Base/Exception.h"

namespace win
{
	class WindowsError :
		public Exception
	{
	public:
		WindowsError(DWORD error);

		virtual void print(std::ostream& os) const override;

	private:
		DWORD m_error;
	};
}

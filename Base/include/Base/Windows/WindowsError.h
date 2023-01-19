#pragma once

#include "Base/Windows/Common.h"

#include <system_error>

#include "Base/Exception.h"

namespace base
{
	namespace win
	{
		class WindowsError :
			public Exception
		{
		public:
			explicit WindowsError(DWORD value);

			virtual void toStream(std::ostream& os) const override;

		private:
			std::error_code m_ec;
		};
	}
}

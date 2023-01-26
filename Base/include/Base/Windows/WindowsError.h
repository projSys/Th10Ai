#pragma once

#include "Base/Windows/Common.h"

#include <system_error>

#include "Base/Throw.h"

namespace base
{
	namespace win
	{
		class WindowsError :
			public Throwable
		{
		public:
			explicit WindowsError(const std::error_code& ec);
			explicit WindowsError(DWORD value,
				const std::error_category& category = std::system_category());

			virtual void toStream(std::ostream& os) const override;

			const std::error_code& getErrorCode() const;

		private:
			std::error_code m_ec;
		};
	}
}

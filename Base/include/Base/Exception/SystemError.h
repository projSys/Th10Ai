#pragma once

#include "Base/Windows/Common.h"

#include <system_error>

#include "Base/Types.h"
#include "Base/Exception/Throw.h"

namespace base
{
	namespace win
	{
		class SystemError :
			public Throwable
		{
		public:
			explicit SystemError(int32_t value,
				const std::error_category& category = std::system_category());
			explicit SystemError(const std::error_code& ec);

			virtual void toStream(std::ostream& os) const override;

			int32_t getValue() const;
			const std::error_code& getErrorCode() const;

		private:
			std::error_code m_ec;
		};
	}
}

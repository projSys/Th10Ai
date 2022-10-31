#pragma once

#include "Base/Common.h"

#include <system_error>

#include "Base/Types.h"
#include "Base/Throwable.h"

namespace base
{
	class Error :
		public std::system_error,
		public Throwable
	{
	public:
		Error(int_t code, bool hex,
			const std::error_category& category = std::system_category(),
			const std::source_location& sl = std::source_location::current());
		explicit Error(int_t code,
			const std::error_category& category = std::system_category(),
			const std::source_location& sl = std::source_location::current());

		virtual void toStream(std::ostream& os) const override;

	private:
		bool m_hex;
	};
}

#pragma once

#include "Base/Common.h"

#include <stdexcept>

#include "Base/Throwable.h"

namespace base
{
	class Exception :
		public std::runtime_error,
		public Throwable
	{
	public:
		Exception(const char* str,
			const std::source_location& loc = std::source_location::current());
		Exception(const std::string& str,
			const std::source_location& loc = std::source_location::current());

		virtual void toStream(std::ostream& os) const override;
	};
}

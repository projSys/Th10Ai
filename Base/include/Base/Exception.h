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
		explicit Exception(const char* str,
			const std::source_location& sl = std::source_location::current());
		explicit Exception(const std::string& str,
			const std::source_location& sl = std::source_location::current());

		virtual void toStream(std::ostream& os) const override;
	};
}

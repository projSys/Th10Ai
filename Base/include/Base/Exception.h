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
			const boost::source_location& loc);
		Exception(const std::string& str,
			const boost::source_location& loc);

		virtual void toStream(std::ostream& os) const override;
	};
}

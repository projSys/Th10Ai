#pragma once

#include "Base/Common.h"

#include <stdexcept>

#include "Base/Exception/Throw.h"

namespace base
{
	class Exception :
		public std::runtime_error,
		public Throwable
	{
	public:
		explicit Exception(const char* str);
		explicit Exception(const std::string& str);

		virtual void toStream(std::ostream& os) const override;
	};
}

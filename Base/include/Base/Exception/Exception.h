#pragma once

#include "Base/Common.h"

#include <stdexcept>

#include "Base/Exception/Throw.h"

namespace base
{
	class Exception :
		public Throwable
	{
	public:
		explicit Exception(const char* str);
		explicit Exception(const std::string& str);

		virtual void toStream(std::ostream& os) const override;

		const char* what() const;

	private:
		std::runtime_error m_impl;
	};
}

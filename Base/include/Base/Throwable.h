#pragma once

#include "Base/Common.h"

#include <source_location>

#include "Base/Streamable.h"

namespace base
{
	class Throwable :
		public OStreamable
	{
	public:
		explicit Throwable(const std::source_location& loc = std::source_location::current());
		virtual ~Throwable() = default;

		virtual void toStream(std::ostream& os) const override;

	private:
		std::source_location m_loc;
	};
}

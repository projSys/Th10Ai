#pragma once

#include "Base/Common.h"

#include <source_location>

#include "Base/Streamable.h"
#include "Base/StackTrace.h"

namespace base
{
	class Throwable :
		public OStreamable
	{
	public:
		explicit Throwable(const std::source_location& sl = std::source_location::current(),
			const StackTrace& st = StackTrace());
		virtual ~Throwable() = default;

		virtual void toStream(std::ostream& os) const override;

	private:
		std::source_location m_sl;
		StackTrace m_st;
	};
}

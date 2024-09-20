#pragma once

#include "Base/Windows/Common.h"

#include "Base/Streamable.h"

namespace base
{
	namespace win
	{
		// https://stackoverflow.com/questions/691719/how-to-display-a-stack-trace-when-an-exception-is-thrown
		// https://github.com/JochenKalmbach/StackWalker
		// https://github.com/jeremy-rifkin/cpptrace
		// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2490r3.html 等C++26
		class StackTrace :
			public OStreamable
		{
		public:
			static StackTrace Current(uint_t skip = 0);
			static StackTrace FromCurrentException(uint_t skip = 2);

			StackTrace();
			StackTrace(const CONTEXT& context, uint_t skip);
			virtual ~StackTrace() = default;

			virtual void toStream(std::ostream& out) const override;

		private:
			CONTEXT m_context;
			uint_t m_skip;
		};
	}
}

#pragma once

#include "Base/Common.h"

// 修改此项需要重新编译全部代码
//#ifndef BASE_THROW_STACKTRACE
//#define BASE_THROW_STACKTRACE
//#endif

#include <iosfwd>
#ifdef BASE_THROW_STACKTRACE
//#include <stacktrace>	// C++23
#include <boost/stacktrace.hpp>
#endif

#include "Base/Types.h"

namespace base
{
#ifdef BASE_THROW_STACKTRACE
	namespace bst = boost::stacktrace;
#endif

	// https://stackoverflow.com/questions/691719/how-to-display-a-stack-trace-when-an-exception-is-thrown
	// https://github.com/JochenKalmbach/StackWalker
	// https://github.com/jeremy-rifkin/cpptrace
	// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2490r3.html 等C++26
	class StackTrace
#ifdef BASE_THROW_STACKTRACE
		: public bst::stacktrace
#endif
	{
	public:
#ifdef BASE_THROW_STACKTRACE
		using bst::stacktrace::stacktrace;
#else
		StackTrace() {}
		StackTrace(uint_t, uint_t) {}
#endif
	};

	std::ostream& operator <<(std::ostream& os, const StackTrace& st);
}

#pragma once

#include "Base/Common.h"

// 修改此项需要全部代码重新编译
//#ifndef BASE_THROW_STACKTRACE
//#define BASE_THROW_STACKTRACE
//#endif

#include <iosfwd>
#ifdef BASE_THROW_STACKTRACE
//#include <stacktrace>	// C++23
#include <boost/stacktrace.hpp>
#endif

namespace base
{
#ifdef BASE_THROW_STACKTRACE
	namespace bst = boost::stacktrace;
#endif

	class StackTrace
#ifdef BASE_THROW_STACKTRACE
		: public bst::stacktrace
#endif
	{
	public:
#ifdef BASE_THROW_STACKTRACE
		using bst::stacktrace::stacktrace;
#else
		StackTrace() = default;
		StackTrace(const StackTrace&) = default;
		StackTrace(StackTrace&&) = default;
		StackTrace(size_t, size_t) {}
#endif
	};

	std::ostream& operator <<(std::ostream& os, const StackTrace& st);
}

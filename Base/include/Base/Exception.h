#pragma once

#include "Base/Common.h"

#include <stdexcept>
#include <ostream>

#include "Base/SourceLocation.h"
#include "Base/StackTrace.h"

namespace base
{
	class Exception :
		public std::runtime_error
	{
	public:
		explicit Exception(const char* str);
		explicit Exception(const std::string& str);
		virtual ~Exception() = default;

		virtual void print(std::ostream& os) const;
	};

	extern thread_local SourceLocation tl_sourceLocation;
	extern thread_local StackTrace tl_stackTrace;

	template <typename T>
	void Throw(const T& ex,
		const SourceLocation& sourceLocation,
		const StackTrace& stackTrace = StackTrace(2))
	{
		tl_sourceLocation = sourceLocation;
		tl_stackTrace = stackTrace;
		throw ex;
	}

#define BASE_THROW(ex) base::Throw(ex, BASE_CURRENT_LOCATION);

	// 只能在catch里调用
	std::string PrintException();
}

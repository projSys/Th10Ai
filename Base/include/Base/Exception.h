#pragma once

#include "Base/Common.h"

#include <type_traits>
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

	protected:
		static thread_local SourceLocation stl_sourceLocation;
		static thread_local StackTrace stl_stackTrace;

		template <typename T>
		friend void Throw(const T& ex,
			const SourceLocation& sourceLocation,
			const StackTrace& stackTrace);
	};

	template <typename T>
	void Throw(const T& ex,
		const SourceLocation& sourceLocation,
		const StackTrace& stackTrace)
	{
		static_assert(std::is_base_of_v<Exception, T>,
			"Parameter must inherit from Exception.");

		Exception::stl_sourceLocation = sourceLocation;
		Exception::stl_stackTrace = stackTrace;
		throw ex;
	}

#define BASE_THROW(ex) \
	base::Throw(ex, BASE_CURRENT_LOCATION, base::StackTrace(2));

	// 只能在catch里调用
	std::string PrintException();
}

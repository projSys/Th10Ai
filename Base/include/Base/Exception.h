#pragma once

#include "Base/Common.h"

#include <stdexcept>
#include <ostream>

#include "Base/Types.h"
#include "Base/String.h"
#include "Base/SourceLocation.h"
#ifdef BASE_WINDOWS
#include "Base/Windows/StackTrace.h"
#endif

namespace base
{
	class Exception :
		public std::runtime_error
	{
	public:
		explicit Exception(const char* ptr = nullptr, uint_t framesToSkip = 0);
		explicit Exception(const std::string& str, uint_t framesToSkip = 0);
		virtual ~Exception() = default;

		virtual void printTo(std::ostream& os) const;

	protected:
		SourceLocation m_sourceLocation;
#ifdef BASE_WINDOWS
		win::StackTrace m_stackTrace;
#endif
	};

#define BASE_THROW(ex) { STORE_SOURCE_LOCATION; throw ex; }

	// 只能在catch里调用
	std::string PrintException();
}

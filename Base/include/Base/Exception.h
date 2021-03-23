#pragma once

#include "Base/Common.h"

#include <stdexcept>
#include <ostream>

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
		explicit Exception(const char* str);
		explicit Exception(const std::string& str);
		virtual ~Exception() = default;

		virtual void print(std::ostream& os) const;

	protected:
		SourceLocation m_sourceLocation;
#ifdef BASE_WINDOWS
		win::StackTrace m_stackTrace;
#endif
	};

#ifdef BASE_WINDOWS
#define BASE_THROW(ex) { \
	base::SourceLocation sourceLocation(__FUNCTION__, __FILE__, __LINE__); \
	base::win::StackTrace stackTrace; \
	throw ex; }
#else
#define BASE_THROW(ex) { \
	base::SourceLocation sourceLocation(__FUNCTION__, __FILE__, __LINE__); \
	throw ex; }
#endif

	// 只能在catch里调用
	std::string PrintException();
}

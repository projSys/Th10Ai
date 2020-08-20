#pragma once

#include <stdexcept>
#include <ostream>
#ifdef _DEBUG
#include <boost/stacktrace.hpp>
#endif

#include "Base/SourceLocation.h"

namespace base
{
	class Exception :
		public std::runtime_error
	{
	public:
		Exception();
		explicit Exception(const char* str);
		explicit Exception(const std::string& str);

		virtual void print(std::ostream& os) const;

	protected:
		void printSourceLocation(std::ostream& os) const;
		void printStackTrace(std::ostream& os) const;

		SourceLocation m_sourceLocation;
#ifdef _DEBUG
		boost::stacktrace::stacktrace m_stackTrace;
#endif
	};

#define BASE_THROW(ex) { STORE_SOURCE_LOCATION; throw ex; }

	// 只能在catch块里调用
	std::string PrintException();
}

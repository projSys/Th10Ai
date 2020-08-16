#pragma once

#include <stdexcept>
#include <ostream>
#ifdef _DEBUG
#include <boost/stacktrace.hpp>
#endif

#include "Base/Types.h"
#include "Base/SourceLocation.h"

namespace base
{
	class Exception :
		public std::runtime_error
	{
	public:
		Exception();
		explicit Exception(const std::string& desc);
		explicit Exception(const char* desc);
		Exception(int_t id, const std::string& desc);
		Exception(int_t id, const char* desc);

		virtual void print(std::ostream& os) const;

	protected:
		void printSourceLocation(std::ostream& os) const;
		void printStackTrace(std::ostream& os) const;

		int_t m_id;
		SourceLocation m_sourceLocation;
#ifdef _DEBUG
		boost::stacktrace::stacktrace m_stackTrace;
#endif
	};

#define BASE_THROW(ex) { STORE_SOURCE_LOCATION; throw ex; }

	// 只能在catch块里调用
	std::string PrintException();
}

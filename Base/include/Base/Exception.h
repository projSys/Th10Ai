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
		Exception(const std::string& whatArg);
		Exception(const char* whatArg);

		virtual void print(std::ostream& os) const;

	protected:
		void printSourceLocation(std::ostream& os) const;
		void printStackTrace(std::ostream& os) const;

		SourceLocation m_sourceLocation;
#ifdef _DEBUG
		boost::stacktrace::stacktrace m_stackTrace;
#endif
	};

#define BASE_THROW_EXCEPTION(ex) { STORE_SOURCE_LOCATION; throw ex; }

	inline std::ostream& operator <<(std::ostream& os, const Exception& ex)
	{
		ex.print(os);
		return os;
	}

	void PrintAllException(std::ostream& os);
}

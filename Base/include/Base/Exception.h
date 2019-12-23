#pragma once

#include <stdexcept>
#include <ostream>
#ifdef _DEBUG
#include <boost/stacktrace.hpp>
#endif

#include "Base/Type.h"

namespace base
{
	class Exception :
		public std::runtime_error
	{
	public:
		Exception(const std::string& whatArg,
			const char* func, const char* file, uint_t line,
			uint_t skip = 3);
		Exception(const char* whatArg,
			const char* func, const char* file, uint_t line,
			uint_t skip = 3);

		virtual void print(std::ostream& os) const;

		const char* getFunc() const;
		const char* getFile() const;
		uint_t getLine() const;

	protected:
		const char* m_func;
		const char* m_file;
		uint_t m_line;
#ifdef _DEBUG
		boost::stacktrace::stacktrace m_stackTrace;
#endif
	};

#define THROW_BASE_EXCEPTION(what) \
	throw base::Exception(what, __FUNCTION__, __FILE__, __LINE__)

	inline std::ostream& operator <<(std::ostream& os, const Exception& ex)
	{
		ex.print(os);
		return os;
	}

	void PrintAllException(std::ostream& os);
}

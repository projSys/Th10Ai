#pragma once

#include <stdexcept>
#include <ostream>
#ifdef _DEBUG
#include <boost/stacktrace.hpp>
#endif

#include "Base/Type.h"

namespace base
{
#ifdef _DEBUG
	namespace bst = boost::stacktrace;
#endif

	class Exception :
		public std::runtime_error
	{
	public:
		Exception(const std::string& whatArg,
			const char* func, const char* file, uint_t line);
		Exception(const char* whatArg,
			const char* func, const char* file, uint_t line);

		virtual void print(std::ostream& os) const;

		const char* getFunc() const;
		const char* getFile() const;
		uint_t getLine() const;

	protected:
		const char* m_func;
		const char* m_file;
		uint_t m_line;
#ifdef _DEBUG
		bst::stacktrace m_stackTrace;
#endif
	};

#define THROW_BASE_EXCEPTION(what) \
	throw base::Exception(what, __func__, __FILE__, __LINE__)
}

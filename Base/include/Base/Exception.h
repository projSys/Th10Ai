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
		Exception(const std::string& whatArg);
		Exception(const char* whatArg);

		virtual void print(std::ostream& os) const;
	};

	class ExceptionExtra
	{
	public:
		ExceptionExtra(const char* func, const char* file, uint_t line);

		void print(std::ostream& os) const;

	private:
		const char* m_func;
		const char* m_file;
		uint_t m_line;
#ifdef _DEBUG
		boost::stacktrace::stacktrace m_stackTrace;
#endif
	};

	template <typename T>
	class ExceptionPackage :
		public T
	{
	public:
		ExceptionPackage(T&& ex,
			const char* func, const char* file, uint_t line) :
			T(std::forward<T>(ex)),
			m_extra(func, file, line)
		{
		}

		virtual void print(std::ostream& os) const override
		{
			T::print(os);
			m_extra.print(os);
		}

	private:
		ExceptionExtra m_extra;
	};

	template <typename T,
		typename = std::enable_if_t<std::is_base_of<Exception, T>::value>>
		inline void ThrowException(T&& ex,
			const char* func, const char* file, uint_t line)
	{
		throw ExceptionPackage<T>(std::forward<T>(ex), func, file, line);
	}

#define BASE_THROW_EXCEPTION(ex) \
	base::ThrowException(ex, __FUNCTION__, __FILE__, __LINE__)

	inline std::ostream& operator <<(std::ostream& os, const Exception& ex)
	{
		ex.print(os);
		return os;
	}

	void PrintAllException(std::ostream& os);
}

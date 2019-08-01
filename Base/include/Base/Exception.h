#pragma once

#include <exception>
#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>

#include "Base/Type.h"

namespace base
{
	class Exception :
		public virtual std::exception,
		public virtual boost::exception
	{
	public:
		virtual const char* what() const override;
	};

	typedef boost::error_info<struct tag_err_no, int_t> err_no;
	typedef boost::error_info<struct tag_err_hex, std::string> err_hex;
	typedef boost::error_info<struct tag_err_str, std::string> err_str;
	typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

#define THROW_BASE_EXCEPTION(ex) \
	BOOST_THROW_EXCEPTION(ex << base::traced(boost::stacktrace::stacktrace()))
}

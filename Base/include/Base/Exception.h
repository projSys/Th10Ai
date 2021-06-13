#pragma once

#include "Base/Common.h"

#include <stdexcept>
#include <ostream>
#include <boost/assert/source_location.hpp>

namespace base
{
	class Throwable
	{
	public:
		explicit Throwable(const boost::source_location& loc);
		virtual ~Throwable() = default;

		virtual void print(std::ostream& os) const;

	private:
		boost::source_location m_loc;
	};

	class Exception :
		public std::runtime_error,
		public Throwable
	{
	public:
		Exception(const char* str,
			const boost::source_location& loc);
		Exception(const std::string& str,
			const boost::source_location& loc);

		virtual void print(std::ostream& os) const override;
	};

#define BASE_THROW(T, ...) \
	throw T(__VA_ARGS__, BOOST_CURRENT_LOCATION)

	// 只能在catch里调用
	std::string PrintException();
}

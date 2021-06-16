#pragma once

#include "Base/Common.h"

#include <boost/assert/source_location.hpp>

#include "Base/Streamable.h"
#include "Base/String.h"

namespace base
{
	class Throwable :
		public OStreamable
	{
	public:
		explicit Throwable(const boost::source_location& loc);
		virtual ~Throwable() = default;

		virtual void toStream(std::ostream& os) const override;

	private:
		boost::source_location m_loc;
	};

#define BASE_THROW(T, ...) \
	throw T(__VA_ARGS__, BOOST_CURRENT_LOCATION)

	// 只能在catch里调用
	std::string PrintException();
}

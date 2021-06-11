#pragma once

#include "Base/Common.h"

#include <boost/log/trivial.hpp>

namespace base
{
	// BASE_LOG(trace)
	// BASE_LOG(debug)
	// BASE_LOG(info)
	// BASE_LOG(warning)
	// BASE_LOG(error)
	// BASE_LOG(fatal)
#define BASE_LOG(level) BOOST_LOG_TRIVIAL(level)
}

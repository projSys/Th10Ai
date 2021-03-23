#pragma once

#include <Base/Common.h>
#include <Base/Types.h>
#include <Base/String.h>
#include <Base/Exception.h>
#include <Base/Logger.h>
#include <Base/Windows/Common.h>

namespace th
{
	using namespace base;
	using base::float_t;
	using namespace base::win;
}

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <iostream>

//#pragma warning(disable:4244)
//#pragma warning(disable:4305)

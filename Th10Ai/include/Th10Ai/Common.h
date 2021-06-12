#pragma once

#include <Base/Common.h>
#include <Base/Types.h>
#include <Base/Exception.h>
#include <Base/Log.h>
#include <Base/Windows/Common.h>
#include <Base/Windows/SystemError.h>

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

#define RENDER 0

#pragma once

#ifdef _WIN32
// define something for Windows (32-bit and 64-bit, this part is common)
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <sdkddkver.h>
#include <WinSock2.h>
#include <Windows.h>

#ifndef BOOST_USE_WINDOWS_H
#define BOOST_USE_WINDOWS_H
#endif

#define BASE_WINDOWS 1

#ifdef _WIN64
// define something for Windows (64-bit only)
#define BASE_64BIT 1
#define BASE_BITS 64
#else
// define something for Windows (32-bit only)
#define BASE_32BIT 1
#define BASE_BITS 32
#endif // _WIN64
#endif // _WIN32

namespace base
{
	// 带参数的宏，先对参数做展开，除非宏定义体中包含#或##
	// #或##会阻止参数展开
#define _MACRO_STRINGIFY(a) #a
#define MACRO_STRINGIFY(a) _MACRO_STRINGIFY(a)

#define _MACRO_CONCAT(a, b) a ## b
#define MACRO_CONCAT(a, b) _MACRO_CONCAT(a, b)
}

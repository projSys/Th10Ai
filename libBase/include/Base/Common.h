#pragma once

// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
// https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
#if defined(_WIN32) //|| defined(WIN32) || defined(__WIN32__) || defined(__NT__)
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

#if defined(_WIN64)
// define something for Windows (64-bit only)
#define BASE_64BIT 1
#define BASE_BITS 64

#else
// define something for Windows (32-bit only)
#define BASE_32BIT 1
#define BASE_BITS 32

#endif

#elif __APPLE__
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform."
#endif
#elif __linux__
// linux
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler."
#endif

namespace base
{
	// 带参数的宏，先对参数做展开，除非宏定义体中包含#或##
	// #或##会阻止参数展开
#define MACRO_STRINGIFY_INNER(a) #a
#define MACRO_STRINGIFY(a) MACRO_STRINGIFY_INNER(a)

#define MACRO_CONCAT_INNER(a, b) a ## b
#define MACRO_CONCAT(a, b) MACRO_CONCAT_INNER(a, b)
}

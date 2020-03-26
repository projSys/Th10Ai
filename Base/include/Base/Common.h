#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#ifndef BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
//#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
//#endif

//#ifndef _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
//#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
//#endif

// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
// http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <sdkddkver.h>
#include <WinSock2.h>
#include <Windows.h>
#define BASE_WINDOWS 1
#ifdef _WIN64
//define something for Windows (64-bit only)
#define BASE_64BIT 1
#define BASE_BITS 64
#else
//define something for Windows (32-bit only)
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
#error "Unknown Apple platform"
#endif
#elif __linux__
// linux
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif

namespace base
{
}

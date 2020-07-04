#pragma once

#include "Windows/Common.h"

#include <memory>

namespace win
{
	using HANDLE_ptr = unique_ptr<remove_pointer_t<HANDLE>, decltype(&CloseHandle)>;
	using HMODULE_ptr = unique_ptr<remove_pointer_t<HMODULE>, decltype(&FreeLibrary)>;
	using HWND_ptr = unique_ptr<remove_pointer_t<HWND>, decltype(&DestroyWindow)>;
}

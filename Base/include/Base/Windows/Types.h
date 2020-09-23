#pragma once

#include "Base/Windows/Common.h"

#include <memory>

namespace base
{
	namespace win
	{
		using HANDLE_ptr = std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype(&CloseHandle)>;
		using HMODULE_ptr = std::unique_ptr<std::remove_pointer_t<HMODULE>, decltype(&FreeLibrary)>;
		using HWND_ptr = std::unique_ptr<std::remove_pointer_t<HWND>, decltype(&DestroyWindow)>;
	}
}

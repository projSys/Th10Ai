#pragma once

#include "Base/Windows/Common.h"

#include <filesystem>

namespace base
{
	namespace win
	{
		namespace fs = std::filesystem;

		class Apis
		{
		public:
			static fs::path GetModulePath(HMODULE module = nullptr);
			static fs::path GetModuleDir(HMODULE module = nullptr);

		private:
			static constexpr DWORD BUFFER_SIZE = 2 * 1024;
		};
	}
}

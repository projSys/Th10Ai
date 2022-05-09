#pragma once

#include "Loader/Common.h"

#include <filesystem>

namespace ld
{
	namespace fs = std::filesystem;

	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(HANDLE process, const fs::path& dllPath);
	};
}

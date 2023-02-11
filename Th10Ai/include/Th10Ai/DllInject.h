#pragma once

#include "Th10Ai/Common.h"

#include <filesystem>

namespace th
{
	namespace fs = std::filesystem;

	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(HANDLE process, const fs::path& dllPath);
	};
}

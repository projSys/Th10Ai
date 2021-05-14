#pragma once

#include "Th10Ai/Common.h"

#include <boost/filesystem.hpp>
#include <Base/Windows/Process.h>

namespace th
{
	namespace fs = boost::filesystem;

	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(Process& process, const fs::path& dllPath);
	};
}

#pragma once

#include "Loader/Common.h"

#include <boost/filesystem.hpp>

namespace ld
{
	namespace fs = boost::filesystem;

	class DllInject
	{
	public:
		static void EnableDebugPrivilege();
		static void Inject(HANDLE process, const fs::path& dllPath);
	};
}

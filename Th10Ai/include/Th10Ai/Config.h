#pragma once

#include "Th10Ai/Common.h"

#include <filesystem>

namespace th
{
	namespace fs = std::filesystem;

	class Config
	{
	public:
		Config();

	public:
		fs::path th10Path;
	};
}

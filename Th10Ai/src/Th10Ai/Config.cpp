#include "Th10Ai/Config.h"

#include <fstream>
#include <boost/program_options.hpp>

#include <Base/Windows/Apis.h>

namespace th
{
	namespace po = boost::program_options;

	Config::Config()
	{
		fs::path dir = Apis::GetModuleDir();
		fs::path confPath = dir / L"Th10Ai.conf";
		std::ifstream ifs(confPath.c_str());
		po::options_description desc("Config");
		desc.add_options()
			("Th10Path", po::value<std::string>(), "Th10Path");
		po::variables_map vm;
		po::store(po::parse_config_file(ifs, desc), vm);
		po::notify(vm);

		th10Path = Apis::AnsiToWide(vm["Th10Path"].as<std::string>());
	}
}

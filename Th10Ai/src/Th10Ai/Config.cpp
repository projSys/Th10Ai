#include "Th10Ai/Config.h"

#include <fstream>
#include <boost/program_options.hpp>
#include <boost/locale.hpp>

#include <Base/Windows/Apis.h>

namespace th
{
	namespace bpo = boost::program_options;
	namespace blc = boost::locale::conv;

	Config::Config()
	{
		fs::path dir = Apis::GetModuleDir();
		fs::path confPath = dir / L"Th10Ai.conf";
		std::ifstream ifs(confPath.native());
		bpo::options_description desc("Config");
		desc.add_options()
			("Th10Path", bpo::value<std::string>(), "Th10Path");
		bpo::variables_map vm;
		bpo::store(bpo::parse_config_file(ifs, desc), vm);
		bpo::notify(vm);

		th10Path = blc::to_utf<wchar_t>(vm["Th10Path"].as<std::string>(), "UTF-8");
	}
}

#include "Loader/Common.h"

#include <fstream>
#include <system_error>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <Base/ScopeGuard.h>
#include <Base/Windows/Apis.h>

#include "Loader/DllInject.h"

namespace po = boost::program_options;
using namespace ld;

Log g_log;

int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPWSTR cmdLine, _In_ int cmdShow)
{
	try
	{
		fs::path logPath = Apis::GetModuleDir() / L"Loader_%N.log";
		g_log.addFileLog(logPath);

		fs::path confPath = Apis::GetModuleDir() / L"Loader.conf";
		std::ifstream ifs(confPath.c_str());
		po::options_description desc("Configuration");
		desc.add_options()
			("exe-path", po::value<std::string>(), "exe path")
			("dll-name", po::value<std::string>(), "dll name");
		po::variables_map vm;
		po::store(po::parse_config_file(ifs, desc), vm);
		po::notify(vm);

		fs::path exePath = Apis::AnsiToWide(vm["exe-path"].as<std::string>());
		fs::path dllName = Apis::AnsiToWide(vm["dll-name"].as<std::string>());
		fs::path exeDir = exePath.parent_path();

		STARTUPINFOW si = {};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi = {};
		if (!CreateProcessW(exePath.c_str(), nullptr, nullptr, nullptr, FALSE,
			CREATE_SUSPENDED, nullptr, exeDir.c_str(), &si, &pi))
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));
		ON_SCOPE_EXIT([&]()
			{
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			});

		fs::path dllPath = Apis::GetModuleDir() / dllName;
		//DllInject::EnableDebugPrivilege();
		DllInject::Inject(pi.hProcess, dllPath);

		DWORD count = ResumeThread(pi.hThread);
		if (count == (DWORD)-1)
			BASE_THROW(std::system_error(GetLastError(), std::system_category()));
	}
	catch (...)
	{
		BASE_LOG_ERROR(PrintException());
	}
	return 0;
}

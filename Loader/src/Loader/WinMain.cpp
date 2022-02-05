#include "Loader/WinMain.h"

#include <fstream>
#include <boost/program_options.hpp>
#include <Base/Catcher.h>
#include <Base/Logger.h>
#include <Base/ScopeGuard.h>
#include <Base/ErrorCode.h>
#include <Base/Windows/Apis.h>

#include "Loader/DllInject.h"

namespace po = boost::program_options;
using namespace ld;

int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPWSTR cmdLine, _In_ int cmdShow)
{
	try
	{
		fs::path logPath = Apis::GetModuleDir() / L"Loader.log";
		g_logger.addFileLog(logPath);
		g_logger.addCommonAttributes();

		fs::path confPath = Apis::GetModuleDir() / L"Loader.conf";
		std::ifstream ifs(confPath.c_str());
		po::options_description desc("Config");
		desc.add_options()
			("exe-path", po::value<std::string>(), "exe path")
			("dll-name", po::value<std::string>(), "dll name");
		po::variables_map vm;
		po::store(po::parse_config_file(ifs, desc), vm);
		po::notify(vm);

		fs::path exePath = Apis::AnsiToWide(vm["exe-path"].as<std::string>());
		std::wstring dllName = Apis::AnsiToWide(vm["dll-name"].as<std::string>());
		fs::path exeDir = exePath.parent_path();

		STARTUPINFOW si = {};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi = {};
		if (!CreateProcessW(exePath.c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, exeDir.c_str(), &si, &pi))
			throw ErrorCode(GetLastError());
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
			throw ErrorCode(GetLastError());
	}
	catch (...)
	{
		BASE_LOG(error) << Catcher() << std::endl;
	}
	return 0;
}

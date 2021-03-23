#include "Th10Ai/Common.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <Base/Windows/Apis.h>
#include <Base/Windows/Module.h>
#include <Base/Windows/Process.h>
#include <Base/Windows/Thread.h>

#include "Th10Ai/DllInject.h"

int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPWSTR cmdLine, _In_ int cmdShow)
{
	base::Logger logger;
	try
	{
		std::string logPath = base::win::Module().getDir() + "/Th10Ai_%N.log";
		logger.addFileLog(logPath);

		std::ifstream ifs("Th10Ai.conf");
		std::string appPath;
		std::getline(ifs, appPath);
		std::wstring appPathW = base::win::Apis::AnsiToWide(appPath);

		boost::filesystem::path p(appPathW);
		std::wstring appDirW = p.remove_filename().wstring();

		STARTUPINFOW si = {};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi = {};
		if (!CreateProcessW(appPathW.c_str(), nullptr, nullptr, nullptr, FALSE,
			CREATE_SUSPENDED, nullptr, appDirW.c_str(), &si, &pi))
			BASE_THROW(base::win::WindowsError());
		base::win::Process process(pi.hProcess);
		base::win::Thread thread(pi.hThread);

		std::string dllPath = base::win::Module().getDir() + "/Th10Hook.dll";

		//th::DllInject::EnableDebugPrivilege();
		th::DllInject::Inject(process, dllPath);

		thread.resume();
	}
	catch (...)
	{
		BASE_LOG_ERROR(base::PrintException());
	}
	return 0;
}

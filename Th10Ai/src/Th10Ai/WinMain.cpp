#include "Th10Ai/Common.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <Base/Windows/Apis.h>
#include <Base/Windows/Module.h>
#include <Base/Windows/Process.h>
#include <Base/Windows/Thread.h>

#include "Th10Ai/DllInject.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	base::Logger logger;
	try
	{
		std::string logPath = base::win::Module().getDir() + "/Th10Ai_%N.log";
		logger.addFileLog(logPath);

		//HWND window = FindWindowW(L"BASE", nullptr);//L"搶曽晽恄榐丂乣 Mountain of Faith. ver 1.00a");
		//if (window == nullptr)
		//	BASE_THROW(base::Exception(u8"东方风神录未运行。"));
		//DWORD processId = 0;
		//DWORD threadId = GetWindowThreadProcessId(window, &processId);

		//base::win::Process target = base::win::Process::Open(PROCESS_ALL_ACCESS, FALSE, processId);

		std::ifstream fin("Th10Ai.conf");
		std::string appPath;
		std::getline(fin, appPath);
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

		th::DllInject::EnableDebugPrivilege();
		th::DllInject::Inject(process, dllPath);

		thread.resume();
	}
	catch (...)
	{
		BASE_LOG_ERROR(base::PrintException());
	}
	return 0;
}

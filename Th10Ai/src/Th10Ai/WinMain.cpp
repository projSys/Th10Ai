#include "Th10Ai/Common.h"

#include <fstream>
#include <Base/Windows/Apis.h>
#include <Base/Windows/Module.h>
#include <Base/Windows/Process.h>
#include <Base/Windows/Thread.h>

#include "Th10Ai/DllInject.h"

using namespace base;
using base::float_t;
using namespace base::win;
using namespace th;

int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPWSTR cmdLine, _In_ int cmdShow)
{
	Logger logger;
	try
	{
		fs::path logPath = Module().getDir() / L"Th10Ai_%N.log";
		logger.addFileLog(logPath);

		std::ifstream ifs("Th10Ai.conf");
		std::string buffer;
		std::getline(ifs, buffer);
		fs::path exePath = Apis::AnsiToWide(buffer);
		fs::path exeDir = exePath.parent_path();
		std::pair<Process, Thread> pi = Process::Create(exePath, nullptr,
			nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, exeDir, nullptr);

		fs::path dllPath = Module().getDir() / L"Th10Hook.dll";
		//DllInject::EnableDebugPrivilege();
		DllInject::Inject(pi.first, dllPath);

		pi.second.resume();
	}
	catch (...)
	{
		BASE_LOG_ERROR(PrintException());
	}
	return 0;
}

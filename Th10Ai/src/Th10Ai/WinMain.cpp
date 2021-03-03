#include "Th10Ai/Common.h"

#include <Base/Windows/Module.h>

#include "Th10Ai/DllInject.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	base::Logger logger;
	try
	{
		std::string logPath = base::win::Module().getDir() + "/Th10Ai_%N.log";
		logger.addFileLog(logPath);

		HWND window = FindWindowW(L"BASE", nullptr);//L"搶曽晽恄榐丂乣 Mountain of Faith. ver 1.00a");
		if (window == nullptr)
			BASE_THROW(base::Exception(u8"东方风神录未运行。"));
		DWORD processId = 0;
		DWORD threadId = GetWindowThreadProcessId(window, &processId);

		th::DllInject::EnableDebugPrivilege();
		base::win::Process target = base::win::Process::Open(PROCESS_ALL_ACCESS, FALSE, processId);
		std::string dllPath = base::win::Module().getDir() + "/Th10Hook.dll";
		th::DllInject::Inject(target, dllPath);
	}
	catch (...)
	{
		BASE_LOG_ERROR(base::PrintException());
	}
	return 0;
}

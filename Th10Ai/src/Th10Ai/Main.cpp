#include "Th10Ai/Common.h"

#include <memory>
#include <Base/Exception/Catcher.h>
#include <Base/Windows/Apis.h>

#include "Th10Ai/Th10Ai.h"

using namespace th;

int main(int argc, char* argv[])
{
	try
	{
		fs::path dir = Apis::GetModuleDir();
		fs::path logPath = dir / L"Th10Ai.log";
		Log::Initialize(logPath);
	}
	catch (...)
	{
		MessageBoxW(nullptr, L"Log初始化失败。", L"Th10Ai", MB_OK);
		throw;
	}

	try
	{
		//HWND console = GetConsoleWindow();
		//HMENU menu = GetSystemMenu(console, FALSE);
		//EnableMenuItem(menu, SC_CLOSE, MF_DISABLED | MF_GRAYED);

		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);

		std::unique_ptr<Th10Ai> th10Ai = std::make_unique<Th10Ai>();
		th10Ai->run();
	}
	catch (...)
	{
		LOG_FATAL() << Catcher() << std::endl;
		Log::Flush();
	}

	system("pause");
	return 0;
}

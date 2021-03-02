#include "Th10Ai/Common.h"

#include <memory>
#include <Base/Windows/Module.h>

#include "Th10Ai/Th10Ai.h"

int main()
{
	base::Logger logger;
	try
	{
		HWND console = GetConsoleWindow();
		HMENU menu = GetSystemMenu(console, FALSE);
		EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

		std::string logPath = base::win::Module().getDir() + "/Th10Ai_%N.log";
		logger.addFileLog(logPath);

		std::unique_ptr<th::Th10Ai> th10Ai = std::make_unique<th::Th10Ai>();
		th10Ai->run();
	}
	catch (...)
	{
		BASE_LOG_ERROR(base::PrintException());
		system("pause");
	}
	return 0;
}

#include "Th10Ai/Common.h"

#include <memory>
#include <Base/Exception/Catcher.h>
//#include "Base/Exception/Exception.h"
//#include "Base/Exception/SystemError.h"

#include "Th10Ai/Th10Ai.h"

int main(int argc, char* argv[])
{
	try
	{
		//HWND console = GetConsoleWindow();
		//HMENU menu = GetSystemMenu(console, FALSE);
		//EnableMenuItem(menu, SC_CLOSE, MF_DISABLED | MF_GRAYED);

		//base::Throw(std::runtime_error("aaaaaaaaaaa"));
		//base::Throw(base::SystemError(ERROR_INSUFFICIENT_BUFFER));
		//base::Thrower<base::Exception>()("ERROR_INSUFFICIENT_BUFFER");

		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);

		std::unique_ptr<th::Th10Ai> th10Ai = std::make_unique<th::Th10Ai>();
		th10Ai->run();
	}
	catch (...)
	{
		BASE_LOG(error) << th::Catcher() << std::endl;
	}
	system("pause");
	return 0;
}

#include "TH10Bot/Common.h"

#include <memory>
#include <chrono>
#include <boost/log/utility/setup/file.hpp>

#include "TH10Bot/TH10Bot.h"

namespace bl = boost::log;

bool IsKeyDown(int vkey)
{
	return (GetAsyncKeyState(vkey) & 0x8000) != 0;
}

int main()
{
	HWND window = GetConsoleWindow();
	HMENU menu = GetSystemMenu(window, FALSE);
	EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

	std::string logName = win::Utils::GetModuleDir() + "/TH10Bot.log";
	bl::add_file_log(logName);

	try
	{
		std::shared_ptr<th::TH10Bot> bot = std::make_shared<th::TH10Bot>();
		std::cout << "请将焦点放在风神录窗口上，开始游戏，然后按A开启Bot，按S停止Bot，按D退出。" << std::endl;
		//int fps = 0;
		//std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
		while (true)
		{
			if (IsKeyDown('A'))
				bot->start();
			if (IsKeyDown('S'))
				bot->stop();
			if (IsKeyDown('D'))
				break;

			bot->update();

			//++fps;
			//std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
			//time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
			//if (e1 >= 1000)
			//{
			//	std::cout << fps << std::endl;
			//	fps = 0;
			//	t0 += std::chrono::milliseconds(1000);
			//}
		}
	}
	catch (...)
	{
		std::string what = boost::current_exception_diagnostic_information();
		BOOST_LOG_TRIVIAL(error) << what;
		std::cout << what << std::endl;
	}

	//system("pause");
	return 0;
}

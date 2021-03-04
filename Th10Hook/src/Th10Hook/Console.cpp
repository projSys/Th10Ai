#include "Th10Hook/Console.h"

#include <Base/Windows/WindowsError.h>

namespace th
{
	Console::Console() :
		m_newStdin(nullptr),
		m_newStdout(nullptr),
		m_newStderr(nullptr)
	{
		if (!AllocConsole())
			BASE_THROW(WindowsError());

		//AttachConsole(ATTACH_PARENT_PROCESS);	// 将当前程序附到父进程上
		//HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);	// 获得控制台输出句柄

		HWND console = GetConsoleWindow();
		HMENU menu = GetSystemMenu(console, FALSE);
		EnableMenuItem(menu, SC_CLOSE, MF_DISABLED | MF_GRAYED);

		freopen_s(&m_newStdin, "CONIN$", "r", stdin);
		freopen_s(&m_newStdout, "CONOUT$", "w", stdout);
		freopen_s(&m_newStderr, "CONOUT$", "w", stderr);

		//_wfreopen_s

		// Clear the error state for all of the C++ standard streams. Attempting to accessing the streams before they refer
		// to a valid target causes the stream to enter an error state. Clearing the error state will fix this problem,
		// which seems to occur in newer version of Visual Studio even when the console has not been read from or written
		// to yet.
		std::cin.clear();
		std::cout.clear();
		std::cerr.clear();
		std::clog.clear();

		std::wcin.clear();
		std::wcout.clear();
		std::wcerr.clear();
		std::wclog.clear();

		//system("chcp 65001");
	}

	Console::~Console()
	{
		fclose(m_newStdin);
		fclose(m_newStdout);
		fclose(m_newStderr);

		FreeConsole();
	}
}

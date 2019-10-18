#include "libTh10Ai/Common.h"
#include "libTh10Ai/libTh10Ai.h"

#include <boost/log/utility/setup/file.hpp>

#include "libTh10Ai/DllMain.h"
#include "libTh10Ai/Th10Ai.h"

// DLL共享数据段
#pragma data_seg("SharedDataSeg")
DWORD g_exeThreadId = 0;
HWND g_window = nullptr;
HHOOK g_hook = nullptr;
bool g_succeeded = false;
bool g_detach = true;
#pragma data_seg()
#pragma comment(linker, "/section:SharedDataSeg,rws")

th::libTh10Ai g_libTh10Ai;

// 在Th10Ai.exe进程运行
void WINAPI Th10AiMain()
{
	if (g_libTh10Ai.attach())
	{
		g_libTh10Ai.wait();
		g_libTh10Ai.detach();
	}
}

namespace th
{
	namespace bl = boost::log;

#define LIBTH10AI_ATTACH (WM_USER + 0x1234)
#define LIBTH10AI_DETACH (WM_USER + 0x1235)

	libTh10Ai::libTh10Ai() :
		Singleton(this)
	{
	}

	bool libTh10Ai::attach()
	{
		std::string logName = Util::GetModuleDir(g_module) + "/Th10Ai_%N.log";
		bl::add_file_log
		(
			bl::keywords::file_name = logName,
			bl::keywords::rotation_size = 10 * 1024 * 1024,
			bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
			bl::keywords::format = "%Message%"
		);

		try
		{
			if (g_exeThreadId != 0)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10Ai已创建。"));

			g_exeThreadId = GetCurrentThreadId();

			g_window = FindWindow(_T("BASE"), nullptr);
			if (g_window == nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"BASE窗口类未找到。"));

			DWORD wndThreadId = GetWindowThreadProcessId(g_window, nullptr);

			g_hook = SetWindowsHookEx(WH_CALLWNDPROC, &libTh10Ai::CallWndProc, g_module, wndThreadId);
			if (g_hook == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());

			SendMessage(g_window, LIBTH10AI_ATTACH, 0, 0);
			if (!g_succeeded)
			{
				UnhookWindowsHookEx(g_hook);
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10Ai创建失败，详细信息请查看libTh10Ai.log。"));
			}

			return true;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
			return false;
		}
	}

	void libTh10Ai::detach()
	{
		if (!g_detach)
			return;

		if (g_succeeded)
		{
			SendMessage(g_window, LIBTH10AI_DETACH, 0, 0);
			g_succeeded = false;
		}
		if (g_hook != nullptr)
		{
			UnhookWindowsHookEx(g_hook);
			g_hook = nullptr;
		}
	}

	void libTh10Ai::wait()
	{
		// GetMessage不会导致鼠标转圈
		MSG msg = {};
		GetMessage(&msg, nullptr, 0, 0);
	}

	void libTh10Ai::notify()
	{
		try
		{
			if (!PostThreadMessage(g_exeThreadId, WM_QUIT, 0, 0))
				THROW_WINDOWS_ERROR(GetLastError());
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	// 在东方窗口线程运行
	// 不能修改消息，只有SendMessage的消息，没有PostMessage的消息
	LRESULT libTh10Ai::CallWndProc(int code, WPARAM wParam, LPARAM lParam)
	{
		libTh10Ai& instance = libTh10Ai::GetInstance();
		return instance.callWndProc(code, wParam, lParam);
	}

	LRESULT libTh10Ai::callWndProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code < 0)
			return CallNextHookEx(g_hook, code, wParam, lParam);

		switch (code)
		{
		case HC_ACTION:
		{
			CWPSTRUCT* cwp = reinterpret_cast<CWPSTRUCT*>(lParam);
			if (cwp != nullptr && cwp->hwnd == g_window)
			{
				switch (cwp->message)
				{
				case LIBTH10AI_ATTACH:
					onAttach();
					break;

				case LIBTH10AI_DETACH:
					onDetach();
					break;

				case WM_DESTROY:
					onDestroy();
					break;
				}
			}
			break;
		}
		}

		return CallNextHookEx(g_hook, code, wParam, lParam);
	}

	void libTh10Ai::onAttach()
	{
		std::string logName = Util::GetModuleDir(g_module) + "/libTh10Ai_%N.log";
		bl::add_file_log
		(
			bl::keywords::file_name = logName,
			bl::keywords::rotation_size = 10 * 1024 * 1024,
			bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
			bl::keywords::format = "%Message%"
		);

		try
		{
			if (m_th10Ai != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10Ai已创建。"));

			m_th10Ai = std::make_shared<Th10Ai>();
			g_succeeded = true;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	void libTh10Ai::onDetach()
	{
		m_th10Ai = nullptr;
	}

	void libTh10Ai::onDestroy()
	{
		g_detach = false;
		m_th10Ai = nullptr;
	}
}

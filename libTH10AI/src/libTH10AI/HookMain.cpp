#include "libTH10AI/Common.h"
#include "libTH10AI/HookMain.h"

#include <memory>
#include <thread>
#include <chrono>
#include <boost/log/utility/setup/file.hpp>

#include "libTH10AI/DllMain.h"
#include "libTH10AI/HookThread.h"

// DLL共享数据段
#pragma data_seg("SharedDataSeg")
HWND g_window = nullptr;
DWORD g_threadId = 0;
HHOOK g_hook = nullptr;
bool g_succeeded = false;
#pragma data_seg()
#pragma comment(linker, "/section:SharedDataSeg,rws")

namespace th
{
	namespace bl = boost::log;

#define HOOK_CREATE (WM_USER + 0x1234)
#define HOOK_DESTROY (WM_USER + 0x1235)

	LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);

	// 在TH10Ai.exe进程运行
	void WINAPI HookCreate()
	{
		try
		{
			std::string logName = Utils::GetModuleDir(g_module) + "/TH10AI.log";
			bl::add_file_log(logName);

			if (g_hook != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"共享钩子句柄不为空。"));

			g_window = FindWindow(_T("BASE"), nullptr);
			if (g_window == nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"窗口类未找到：BASE。"));

			g_threadId = GetWindowThreadProcessId(g_window, nullptr);

			g_hook = SetWindowsHookEx(WH_CALLWNDPROC, &CallWndProc, g_module, g_threadId);
			if (g_hook == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());

			SendMessage(g_window, HOOK_CREATE, 0, 0);
			if (!g_succeeded)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"TH10AI创建失败，详细信息请查看libTH10AI.log。"));

			MSG msg = {};
			while (true)
			{
				PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
				if (GetAsyncKeyState('D') & 0x8000)
				{
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
			HookDestroy();
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	void WINAPI HookDestroy()
	{
		if (g_succeeded)
		{
			BOOST_LOG_TRIVIAL(error) << "g_succeeded";
			SendMessage(g_window, HOOK_DESTROY, 0, 0);
			g_succeeded = false;
		}
		if (g_hook != nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "g_hook";
			BOOST_LOG_TRIVIAL(error) << UnhookWindowsHookEx(g_hook);
			g_hook = nullptr;
		}
		g_threadId = 0;
		g_window = nullptr;
	}

	std::shared_ptr<HookThread> g_hookThread;

	// 在东方窗口线程运行
	// 不能修改消息，只有SendMessage消息，没有PostMessage消息
	LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam)
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
				case HOOK_CREATE:
					if (g_hookThread == nullptr)
					{
						try
						{
							std::string logName = Utils::GetModuleDir(g_module) + "/libTH10AI.log";
							bl::add_file_log(logName);

							g_hookThread = std::make_shared<HookThread>();
							g_succeeded = true;
						}
						catch (...)
						{
							std::string what = boost::current_exception_diagnostic_information();
							BOOST_LOG_TRIVIAL(error) << what;
						}
					}
					BOOST_LOG_TRIVIAL(error) << "HOOK_CREATE";
					break;

				case HOOK_DESTROY:
					if (g_hookThread != nullptr)
						g_hookThread = nullptr;
					BOOST_LOG_TRIVIAL(error) << "HOOK_DESTROY";
					break;

				case WM_CLOSE:
					BOOST_LOG_TRIVIAL(error) << "WM_CLOSE";
					break;

				case WM_DESTROY:
					BOOST_LOG_TRIVIAL(error) << "WM_DESTROY";
					break;
				}
			}
			break;
		}
		}

		return CallNextHookEx(g_hook, code, wParam, lParam);
	}
}

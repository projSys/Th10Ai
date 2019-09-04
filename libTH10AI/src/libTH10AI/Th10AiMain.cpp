#include "libTH10AI/Common.h"
#include "libTH10AI/Th10AiMain.h"

#include <memory>
#include <chrono>
#include <boost/log/utility/setup/file.hpp>
#include <Base/ScopeGuard.h>

#include "libTH10AI/DllMain.h"
#include "libTH10AI/Th10Ai.h"

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

#define TH10AI_CREATE (WM_USER + 0x1234)
#define TH10AI_DESTROY (WM_USER + 0x1235)

	LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);

	// 在TH10Ai.exe进程运行
	void WINAPI Th10AiMain()
	{
		try
		{
			std::string logName = Utils::GetModuleDir(g_module) + "/TH10AI.log";
			bl::add_file_log(logName);

			if (g_hook != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"共享钩子句柄不为空。"));

			g_window = FindWindow(_T("BASE"), nullptr);
			if (g_window == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());

			g_threadId = GetWindowThreadProcessId(g_window, nullptr);

			g_hook = SetWindowsHookEx(WH_CALLWNDPROC, &CallWndProc, g_module, g_threadId);
			if (g_hook == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());
			ON_SCOPE_EXIT([&]()
			{
				UnhookWindowsHookEx(g_hook);
				g_hook = nullptr;
				g_threadId = 0;
				g_window = nullptr;
			});

			g_succeeded = false;
			SendMessage(g_window, TH10AI_CREATE, 0, 0);
			if (!g_succeeded)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"TH10AI创建失败，详细信息请查看libTH10AI.log。"));
			ON_SCOPE_EXIT([&]()
			{
				SendMessage(g_window, TH10AI_DESTROY, 0, 0);
				g_succeeded = false;
			});

			while (IsWindow(g_window))
			{
				if (GetAsyncKeyState('D') & 0x8000)
					break;
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	std::shared_ptr<Th10Ai> g_th10Ai;

	// 在东方窗口线程运行
	// 不能修改消息，只能处理SendMessage消息，不能处理PostMessage消息
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
				case TH10AI_CREATE:
					if (g_th10Ai == nullptr)
					{
						try
						{
							std::string logName = Utils::GetModuleDir(g_module) + "/libTH10AI.log";
							bl::add_file_log(logName);

							g_th10Ai = std::make_shared<Th10Ai>();
							g_succeeded = true;
						}
						catch (...)
						{
							std::string what = boost::current_exception_diagnostic_information();
							BOOST_LOG_TRIVIAL(error) << what;
						}
					}
					break;

				case TH10AI_DESTROY:
				case WM_CLOSE:
					g_th10Ai = nullptr;
					break;
				}
			}
			break;
		}
		}

		return CallNextHookEx(g_hook, code, wParam, lParam);
	}
}

#include "libTh10Ai/Common.h"
#include "libTh10Ai/libTh10Ai.h"

#include <boost/log/utility/setup/file.hpp>
#include <Base/ScopeGuard.h>

#include "libTh10Ai/DllMain.h"
#include "libTh10Ai/Th10Ai.h"

// DLL共享数据段
#pragma data_seg("SharedDataSeg")
HWND g_window = nullptr;
HHOOK g_hook = nullptr;
bool g_succeeded = false;
bool g_sendDetach = true;
DWORD g_exeThreadId = 0;
#pragma data_seg()
#pragma comment(linker, "/section:SharedDataSeg,rws")

void WINAPI Th10AiMain()
{
	th::libTh10Ai::Main();
}

namespace th
{
	namespace bl = boost::log;

#define LIBTH10AI_ATTACH (WM_USER + 0x1234)
#define LIBTH10AI_DETACH (WM_USER + 0x1235)

	std::shared_ptr<Th10Ai> libTh10Ai::s_th10Ai;

	// 在Th10Ai.exe进程运行
	void libTh10Ai::Main()
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
			if (g_window != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10Ai已创建。"));

			g_window = FindWindow(_T("BASE"), nullptr);
			if (g_window == nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"BASE窗口类未找到。"));
			DWORD wndThreadId = GetWindowThreadProcessId(g_window, nullptr);

			g_hook = SetWindowsHookEx(WH_CALLWNDPROC, &libTh10Ai::CallWndProc, g_module, wndThreadId);
			if (g_hook == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());
			ON_SCOPE_EXIT([&]()
			{
				UnhookWindowsHookEx(g_hook);
			});

			SendMessage(g_window, LIBTH10AI_ATTACH, 0, 0);
			if (!g_succeeded)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10Ai创建失败，详细信息请查看libTh10Ai.log。"));
			ON_SCOPE_EXIT([&]()
			{
				if (g_sendDetach)
					SendMessage(g_window, LIBTH10AI_DETACH, 0, 0);
			});

			// 等待消息退出，GetMessage()不会导致鼠标转圈
			g_exeThreadId = GetCurrentThreadId();
			MSG msg = {};
			GetMessage(&msg, nullptr, 0, 0);
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	void libTh10Ai::Exit()
	{
		try
		{
			// 发送消息退出
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
	// 只有SendMessage()的消息，没有PostMessage()的消息
	// 不能修改消息
	LRESULT libTh10Ai::CallWndProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code < 0)
			return CallNextHookEx(g_hook, code, wParam, lParam);

		switch (code)
		{
		case HC_ACTION:
		{
			CWPSTRUCT* cwpStruct = reinterpret_cast<CWPSTRUCT*>(lParam);
			if (cwpStruct != nullptr && cwpStruct->hwnd == g_window)
			{
				switch (cwpStruct->message)
				{
				case LIBTH10AI_ATTACH:
					OnAttach();
					break;

				case LIBTH10AI_DETACH:
					OnDetach();
					break;

				case WM_DESTROY:
					OnDestroy();
					break;
				}
			}
			break;
		}
		}

		return CallNextHookEx(g_hook, code, wParam, lParam);
	}

	void libTh10Ai::OnAttach()
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
			if (s_th10Ai != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10Ai已创建。"));

			s_th10Ai = std::make_shared<Th10Ai>();
			g_succeeded = true;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	void libTh10Ai::OnDetach()
	{
		s_th10Ai = nullptr;
	}

	void libTh10Ai::OnDestroy()
	{
		g_sendDetach = false;
		s_th10Ai = nullptr;
	}
}

#include "GraphHook/Common.h"
#include "GraphHook/GraphHook.h"

#include <boost/log/utility/setup/file.hpp>

#include "GraphHook/DllMain.h"

// DLL共享数据段
#pragma data_seg("SharedDataSeg")
HWND g_window = nullptr;
DWORD g_threadId = 0;
HHOOK g_hook = nullptr;
bool g_succeeded = false;
#pragma data_seg()
#pragma comment(linker, "/section:SharedDataSeg,rws")

bool WINAPI Hook(HWND window, base::int_t hookType)
{
	gh::GraphHook& graphHook = gh::GraphHook::GetInstance();
	return graphHook.hook(window, hookType);
}

void WINAPI Unhook()
{
	gh::GraphHook& graphHook = gh::GraphHook::GetInstance();
	graphHook.unhook();
}

namespace gh
{
	namespace bl = boost::log;

	// 1次注入会有2个实例 
	GraphHook g_graphHook;

	GraphHook::GraphHook() :
		Singleton(this)
	{
	}

	// 在主进程运行
	bool GraphHook::hook(HWND window, int_t hookType)
	{
		try
		{
			if (g_hook != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"共享钩子句柄不为空。"));

			std::string logName = Utils::GetModuleDir(g_dllModule) + "/GraphHook1.log";
			bl::add_file_log(logName);
			bl::core::get()->set_filter
			(
				bl::trivial::severity >= bl::trivial::info
			);
			BOOST_LOG_TRIVIAL(error) << "11111111111";
			BOOST_LOG_TRIVIAL(error) << logName;

			g_window = window;
			g_threadId = GetWindowThreadProcessId(window, nullptr);
			g_hook = SetWindowsHookEx(WH_CALLWNDPROC, &GraphHook::HookProc, g_dllModule, g_threadId);
			if (g_hook == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());

			g_succeeded = false;
			SendMessage(window, WM_HOOK, hookType, 0);
			return g_succeeded;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
			return false;
		}
	}

	void GraphHook::unhook()
	{
		try
		{
			if (g_hook != nullptr)
			{
				SendMessage(g_window, WM_UNHOOK, 0, 0);

				UnhookWindowsHookEx(g_hook);
				g_succeeded = false;
				g_hook = nullptr;
				g_threadId = 0;
				g_window = nullptr;
			}
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	LRESULT GraphHook::HookProc(int code, WPARAM wParam, LPARAM lParam)
	{
		GraphHook& graphHook = GraphHook::GetInstance();
		return graphHook.hookProc(code, wParam, lParam);
	}

	// 在被注入的窗口线程中运行，不能拦截修改消息
	// 只能处理SendMessage消息，不能处理PostMessage消息
	LRESULT GraphHook::hookProc(int code, WPARAM wParam, LPARAM lParam)
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
				case WM_HOOK:
					onHook(cwp->wParam);
					break;

				case WM_UNHOOK:
					onUnhook();
					break;
				}
			}
			break;
		}

		default:
			break;
		}

		return CallNextHookEx(g_hook, code, wParam, lParam);
	}

	void GraphHook::onHook(int_t hookType)
	{
		try
		{
			if (m_hookIniter != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"重复挂载钩子。"));

			std::string logName = Utils::GetModuleDir(g_dllModule) + "/GraphHook2.log";
			bl::add_file_log(logName);
			BOOST_LOG_TRIVIAL(error) << "22222222222222222";
			BOOST_LOG_TRIVIAL(error) << logName;

			m_hookIniter = std::make_shared<HookIniter>();
			if (hookType & GH_D3D9FRAMESYNC)
				m_d3d9FrameSync = std::make_shared<D3D9FrameSync>();
			if (hookType & GH_D3D9HOOK)
				m_d3d9Hook = std::make_shared<D3D9Hook>();
			if (hookType & GH_D3D10HOOK)
				m_d3d10Hook = std::make_shared<D3D10Hook>();
			if (hookType & GH_D3D11HOOK)
				m_d3d11Hook = std::make_shared<D3D11Hook>();
			if (hookType & GH_DI8HOOK)
				m_di8Hook = std::make_shared<DI8Hook>();

			g_succeeded = true;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	void GraphHook::onUnhook()
	{
		try
		{
			m_di8Hook = nullptr;
			m_d3d11Hook = nullptr;
			m_d3d10Hook = nullptr;
			m_d3d9Hook = nullptr;
			m_d3d9FrameSync = nullptr;
			m_hookIniter = nullptr;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}
}

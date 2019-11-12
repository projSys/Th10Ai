#include "libTh10Ai/Common.h"
#include "libTh10Ai/Util/DllInject.h"

#include <Base/ScopeGuard.h>

#include "libTh10Ai/DllMain.h"

// DLL共享数据段
#pragma data_seg("SharedDataSeg")
extern "C"
{
	__declspec(dllexport) HWND g_window = nullptr;
	__declspec(dllexport) HHOOK g_hook = nullptr;
	__declspec(dllexport) bool g_attached = false;
	__declspec(dllexport) bool g_detach = true;
	__declspec(dllexport) DWORD g_exeThreadId = 0;
}
#pragma data_seg()
#pragma comment(linker, "/section:SharedDataSeg,rws")

namespace th
{
#define DI_ATTACH (WM_USER + 0x1234)
#define DI_DETACH (WM_USER + 0x1235)

	DllInject::DllInject() :
		Singleton(this)
	{
	}

	// 在自身进程运行
	void DllInject::main(LPCTSTR className, LPCTSTR windowName)
	{
		try
		{
			if (g_hook != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str("Dll injected."));

			g_window = FindWindow(className, windowName);
			if (g_window == nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str("Window not found."));
			DWORD wndThreadId = GetWindowThreadProcessId(g_window, nullptr);

			g_hook = SetWindowsHookEx(WH_CALLWNDPROC, &DllInject::CallWndProc, g_module, wndThreadId);
			if (g_hook == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());
			ON_SCOPE_EXIT([&]()
			{
				UnhookWindowsHookEx(g_hook);
			});

			SendMessage(g_window, DI_ATTACH, 0, 0);
			if (!g_attached)
				THROW_BASE_EXCEPTION(Exception() << err_str("Dll Inject failed."));
			ON_SCOPE_EXIT([&]()
			{
				if (g_detach)
					SendMessage(g_window, DI_DETACH, 0, 0);
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

	void DllInject::exit()
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

	// 在目标窗口线程运行
	// 只有SendMessage()的消息，没有PostMessage()的消息
	// 不能修改消息
	LRESULT DllInject::CallWndProc(int code, WPARAM wParam, LPARAM lParam)
	{
		DllInject& instance = DllInject::GetInstance();
		return instance.callWndProc(code, wParam, lParam);
	}

	LRESULT DllInject::callWndProc(int code, WPARAM wParam, LPARAM lParam)
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
				case DI_ATTACH:
					g_attached = onAttach();
					break;

				case DI_DETACH:
					onDetach();
					break;

				case WM_DESTROY:
					g_detach = false;
					onDetach();
					break;
				}
			}
			break;
		}
		}

		return CallNextHookEx(g_hook, code, wParam, lParam);
	}

	bool DllInject::onAttach()
	{
		return false;
	}

	void DllInject::onDetach()
	{
	}
}

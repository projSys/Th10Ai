#include "libTh10Ai/Common.h"
#include "libTh10Ai/HookMain.h"

#include <memory>
#include <boost/log/utility/setup/file.hpp>
#include <Base/ScopeGuard.h>

#include "libTh10Ai/DllMain.h"
#include "libTh10Ai/HookThread.h"

// DLL�������ݶ�
#pragma data_seg("SharedDataSeg")
HWND g_window = nullptr;
DWORD g_threadId = 0;
DWORD g_mainThreadId = 0;
HHOOK g_hook = nullptr;
bool g_created = false;
bool g_destroy = false;
#pragma data_seg()
#pragma comment(linker, "/section:SharedDataSeg,rws")

namespace th
{
	namespace bl = boost::log;

#define HOOK_CREATE (WM_USER + 0x1234)
#define HOOK_DESTROY (WM_USER + 0x1235)

	LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);

	// ��Th10Ai��������
	void WINAPI HookMain()
	{
		try
		{
			std::string logName = Util::GetModuleDir(g_module) + "/Th10Ai_%N.log";
			bl::add_file_log
			(
				bl::keywords::file_name = logName,
				bl::keywords::rotation_size = 10 * 1024 * 1024,
				bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
				bl::keywords::format = "[%TimeStamp%]%Message%"
			);

			if (g_hook != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"��Ϣ�����Ѵ�����"));

			g_window = FindWindow(_T("BASE"), nullptr);
			if (g_window == nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"������δ�ҵ���BASE��"));
			g_threadId = GetWindowThreadProcessId(g_window, nullptr);
			g_mainThreadId = GetCurrentThreadId();

			g_hook = SetWindowsHookEx(WH_CALLWNDPROC, &CallWndProc, g_module, g_threadId);
			if (g_hook == nullptr)
				THROW_WINDOWS_ERROR(GetLastError());
			ON_SCOPE_EXIT([]()
			{
				UnhookWindowsHookEx(g_hook);
				g_hook = nullptr;
			});

			SendMessage(g_window, HOOK_CREATE, 0, 0);
			if (!g_created)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10Ai����ʧ�ܣ���ϸ��Ϣ��鿴libTh10Ai.log��"));
			ON_SCOPE_EXIT([]()
			{
				if (g_destroy)
				{
					SendMessage(g_window, HOOK_DESTROY, 0, 0);
					g_destroy = false;
				}
				g_created = false;
			});

			// GetMessage���ᵼ�����תȦ
			MSG msg = {};
			GetMessage(&msg, nullptr, 0, 0);
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	// �ڹ����߳�����
	void HookExit(bool destroy)
	{
		try
		{
			g_destroy = destroy;
			if (!PostThreadMessage(g_mainThreadId, WM_QUIT, 0, 0))
				THROW_WINDOWS_ERROR(GetLastError());
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	std::shared_ptr<HookThread> g_hookThread;

	// �ڶ��������߳�����
	void HookCreate()
	{
		try
		{
			std::string logName = Util::GetModuleDir(g_module) + "/libTh10Ai_%N.log";
			bl::add_file_log
			(
				bl::keywords::file_name = logName,
				bl::keywords::rotation_size = 10 * 1024 * 1024,
				bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
				bl::keywords::format = "[%TimeStamp%]%Message%"
			);

			if (g_hookThread != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"�����߳��Ѵ�����"));

			g_hookThread = std::make_shared<HookThread>();
			g_created = true;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	// �ڶ��������߳�����
	void HookDestroy()
	{
		if (g_hookThread != nullptr)
			g_hookThread = nullptr;
	}

	// �ڶ��������߳�����
	// �����޸���Ϣ��ֻ��SendMessage����Ϣ��û��PostMessage����Ϣ
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
					HookCreate();
					break;

					// �ɹ����߳����HookExit�����������߳��ѽ�������������Դ
				case HOOK_DESTROY:
					HookDestroy();
					break;

					// �����߳�δ��������Ҫ�ȴ�
				case WM_CLOSE:
					HookDestroy();
					break;

				case WM_DESTROY:
					break;
				}
			}
			break;
		}
		}

		return CallNextHookEx(g_hook, code, wParam, lParam);
	}
}

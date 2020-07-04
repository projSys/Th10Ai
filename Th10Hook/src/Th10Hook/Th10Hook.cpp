#include "Th10Hook/Common.h"
#include "Th10Hook/Th10Hook.h"

#include <sstream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <Base/ScopeGuard.h>

#include "Th10Hook/DllMain.h"
#include "Th10Hook/Th10Ai.h"

namespace th
{
	using namespace boost::interprocess;
	namespace bl = boost::log;

#define TH10_HOOK (WM_USER + 0x1234)
#define TH10_UNHOOK (WM_USER + 0x1235)

	Th10Hook g_th10Hook;

	Th10Hook::Th10Hook() :
		Singleton<Th10Hook>(this),
		m_exitCode(0),
		m_data(nullptr),
		m_window(nullptr),
		m_isUnicode(true),
		m_prevWndProc(nullptr),
		m_isHooked(false)
	{
	}

	int_t Th10Hook::run()
	{
		try
		{
			string logName = Utils::GetModuleDir(g_dll) + "/Th10Hook_%N.log";
			bl::add_file_log
			(
				bl::keywords::file_name = logName,
				bl::keywords::rotation_size = 10 * 1024 * 1024,
				bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
				bl::keywords::format = "%Message%"
			);

			// 打开共享内存
			m_sharedMemory = managed_windows_shared_memory(open_only, "Th10HookMemory");
			ON_SCOPE_EXIT([&]()
				{
					m_sharedMemory = managed_windows_shared_memory();
				});
			m_data = m_sharedMemory.find<Th10HookData>("Th10HookData").first;
			if (m_data == nullptr)
				BASE_THROW(Exception("Th10HookData not found."));

			m_window = m_data->window;

			// 窗口子类化
			m_isUnicode = IsWindowUnicode(m_window);
			if (m_isUnicode)
				m_prevWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(m_window, GWLP_WNDPROC,
					reinterpret_cast<LONG_PTR>(&Th10Hook::WindowProc)));
			else
				m_prevWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(m_window, GWLP_WNDPROC,
					reinterpret_cast<LONG_PTR>(&Th10Hook::WindowProc)));
			if (m_prevWndProc == nullptr)
				BASE_THROW(WindowsError());
			ON_SCOPE_EXIT([&]()
				{
					if (m_exitCode == 0)
					{
						if (m_isUnicode)
							SetWindowLongPtrW(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
						else
							SetWindowLongPtrA(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
					}
				});

			// 加载钩子
			SendMessageW(m_window, TH10_HOOK, 0, 0);
			if (!m_isHooked)
				BASE_THROW(Exception());
			ON_SCOPE_EXIT([&]()
				{
					if (m_exitCode == 0)
						SendMessageW(m_window, TH10_UNHOOK, 0, 0);
				});

			{
				scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
				m_data->isHooked = true;
				m_data->hookCond.notify_one();
			}
			{
				scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
				if (!m_data->isUnhook)
					m_data->unhookCond.wait(lock);
			}
		}
		catch (...)
		{
			ostringstream oss;
			PrintException(oss);
			BOOST_LOG_TRIVIAL(error) << oss.str() << endl;
			m_exitCode = 0;
		}

		BOOST_LOG_TRIVIAL(error) << "555555555555555555555";

		return m_exitCode;
	}

	LRESULT Th10Hook::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		Th10Hook& th10Hook = Singleton<Th10Hook>::GetInstance();
		return th10Hook.windowProc(window, message, wparam, lparam);
	}

	LRESULT Th10Hook::windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		try
		{
			switch (message)
			{
			case TH10_HOOK:
				onHook();
				break;

			case TH10_UNHOOK:
				onUnhook();
				break;

			case WM_DESTROY:
				onDestroy();
				break;
			}
		}
		catch (...)
		{
			ostringstream oss;
			PrintException(oss);
			BOOST_LOG_TRIVIAL(error) << oss.str() << endl;
		}

		if (m_isUnicode)
			return CallWindowProcW(m_prevWndProc, window, message, wparam, lparam);
		else
			return CallWindowProcA(m_prevWndProc, window, message, wparam, lparam);
	}

	void Th10Hook::onHook()
	{
		BOOST_LOG_TRIVIAL(error) << "666666666666666666666";

		m_th10Ai = make_unique<Th10Ai>();
		m_isHooked = true;
	}

	void Th10Hook::onUnhook()
	{
		BOOST_LOG_TRIVIAL(error) << "7777777777777777777";

		m_th10Ai = nullptr;
		m_isHooked = false;
	}

	void Th10Hook::onDestroy()
	{
		BOOST_LOG_TRIVIAL(error) << "8888888888888888888";

		onUnhook();

		m_exitCode = 1;
		{
			scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
			m_data->isUnhook = true;
			m_data->unhookCond.notify_one();
		}
		join();
	}
}

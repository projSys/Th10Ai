#include "Th10Hook/Common.h"
#include "Th10Hook/Th10Context.h"

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

	Th10Context g_th10Context;

	Th10Context::Th10Context() :
		Singleton(this),
		m_data(nullptr),
		m_window(nullptr),
		m_isUnicode(false),
		m_prevWndProc(nullptr),
		m_isSelfExit(true)
	{
	}

	void Th10Context::onDllProcessAttach()
	{
		m_thread = boost::thread(&Th10Context::hookProc, this);
	}

	void Th10Context::hookProc()
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

			ostringstream oss;
			oss << "Th10Hook-" << GetCurrentProcessId();
			m_name = oss.str();
			m_managedShm = managed_shared_memory(open_only, m_name.c_str());
			m_data = m_managedShm.find<Th10Data>("Th10Data").first;
			if (m_data == nullptr)
				BASE_THROW(Exception("Th10Data not found."));

			m_window = m_data->window;

			// 窗口子类化
			m_isUnicode = IsWindowUnicode(m_window);
			if (m_isUnicode)
				m_prevWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(m_window, GWLP_WNDPROC,
					reinterpret_cast<LONG_PTR>(&Th10Context::WindowProc)));
			else
				m_prevWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(m_window, GWLP_WNDPROC,
					reinterpret_cast<LONG_PTR>(&Th10Context::WindowProc)));
			if (m_prevWndProc == nullptr)
				BASE_THROW(WindowsError());
			ON_SCOPE_EXIT([&]()
				{
					if (m_isSelfExit)
					{
						if (m_isUnicode)
							SetWindowLongPtrW(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
						else
							SetWindowLongPtrA(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
					}
				});

			SendMessageW(m_window, TH10_HOOK, 0, 0);
			ON_SCOPE_EXIT([&]()
				{
					if (m_isSelfExit)
						SendMessageW(m_window, TH10_UNHOOK, 0, 0);
				});

			BOOST_LOG_TRIVIAL(error) << "1111111111111111";

			//unique_lock<mutex> lock(m_mutex);
			//m_cond.wait(lock);
			m_th10Ai->controlProc();
		}
		catch (...)
		{
			ostringstream oss;
			PrintException(oss);
			BOOST_LOG_TRIVIAL(error) << oss.str() << endl;
		}

		if (m_isSelfExit)
		{
			m_thread.detach();
			FreeLibraryAndExitThread(g_dll, 0);
		}
	}

	LRESULT Th10Context::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		Th10Context& th10Context = Th10Context::GetInstance();
		return th10Context.windowProc(window, message, wparam, lparam);
	}

	LRESULT Th10Context::windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		assert(window == m_window);

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

	void Th10Context::onHook()
	{
		if (m_th10Ai != nullptr)
			BASE_THROW(Exception(u8"Th10Ai已创建。"));

		m_th10Ai = make_shared<Th10Ai>();
	}

	void Th10Context::onUnhook()
	{
		m_th10Ai = nullptr;
	}

	void Th10Context::onDestroy()
	{
		onUnhook();

		m_isSelfExit = false;
		//unique_lock<mutex> lock(m_mutex);
		//m_cond.notify_one();
		if (m_thread.joinable())
			m_thread.join();
	}
}

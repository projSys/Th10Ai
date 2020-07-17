#include "Th10Hook/Common.h"
#include "Th10Hook/Th10HookLoader.h"

#include <Base/ScopeGuard.h>

#include "Th10Hook/DllMain.h"
#include "Th10Hook/Th10Ai.h"

namespace th
{
#define TH10HL_LOAD (WM_USER + 0x1234)
#define TH10HL_UNLOAD (WM_USER + 0x1235)

	Th10HookLoader g_th10HookLoader;

	Th10HookLoader::Th10HookLoader() :
		m_windowHook(this)
	{
	}

	void Th10HookLoader::run()
	{
		string logName = Utils::GetModuleDir(g_dll) + "/Th10Hook_%N.log";
		m_logger.addFileLog(logName);

		m_context = make_unique<Th10Context>();
		ON_SCOPE_EXIT([&]()
			{
				m_context = nullptr;
			});

		m_windowHook.attach(m_context->getWindow());
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.detach();
			});
		m_windowHook.sendMessage(TH10HL_LOAD, 0, 0);
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.sendMessage(TH10HL_UNLOAD, 0, 0);
			});

		m_context->notifyHook();

		m_context->waitUnhook();

		//while (true)
		//{
		//	if (!m_d3d9Hook->waitPresent())
		//		cout << "CPU太慢了，数据读取不及时。" << endl;

		//	Reader2::ReadPlayer(m_data->status.player);
		//	m_data->status.itemCount = Reader2::ReadItems(m_data->status.items);
		//	m_data->status.enemyCount = Reader2::ReadEnemies(m_data->status.enemies);
		//	m_data->status.bulletCount = Reader2::ReadBullets(m_data->status.bullets);
		//	m_data->status.laserCount = Reader2::ReadLasers(m_data->status.lasers);

		//	scoped_lock<interprocess_mutex> lock(m_data->updateMutex);
		//	m_data->isUpdated = true;
		//	m_data->updateCond.notify_one();
		//}
	}

	void Th10HookLoader::onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case TH10HL_LOAD:
			onLoad();
			break;

		case TH10HL_UNLOAD:
			onUnload();
			break;

		case WM_DESTROY:
			onDestroy();
			break;
		}
	}

	void Th10HookLoader::onLoad()
	{
		m_th10Ai = make_unique<Th10Ai>();
		//m_d3d9Hook = make_unique<D3D9Hook>();
		//m_di8Hook = make_unique<DI8Hook>();
	}

	void Th10HookLoader::onUnload()
	{
		//m_di8Hook = nullptr;
		//m_d3d9Hook = nullptr;
		m_th10Ai = nullptr;
	}

	void Th10HookLoader::onDestroy()
	{
		setSelfExit(false);

		onUnload();
		m_context->notifyUnhook();

		join();
	}
}

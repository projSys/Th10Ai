#include "Th10Ai/Th10Ai.h"

#include <boost/optional.hpp>
#include <Base/Clock.h>

#include "Th10Ai/DllInject.h"
#include "Th10Ai/MyWindow.h"
#include "Th10Ai/Path.h"

namespace th
{
	Th10Ai::Th10Ai(MyWindow* myWindow) :
		m_myWindow(myWindow),
		m_controlDone(false),
		m_controlActive(false),
		m_readDone(false),
		m_readActive(false),
		m_handleDone(false),
		m_handleActive(false),
		m_bombCount(0),
		m_prevDir(DIR::HOLD),
		m_prevSlow(false),
		//m_statusUpdated(false)
		m_bombTime(0)
	{
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);

		HWND console = GetConsoleWindow();
		HMENU menu = GetSystemMenu(console, FALSE);
		EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

		m_sharedMemory = interprocess::managed_windows_shared_memory(
			interprocess::create_only, "Th10-SharedMemory", 8 * 1024 * 1024);
		m_sharedData = m_sharedMemory.construct<SharedData>("Th10-SharedData")();
		if (m_sharedData == nullptr)
			BASE_THROW(Exception(u8"Th10-SharedData名称已被使用。"));

		HWND window = FindWindowW(L"BASE", nullptr);//L"搶曽晽恄榐丂乣 Mountain of Faith. ver 1.00a");
		if (window == nullptr)
			BASE_THROW(Exception(u8"东方风神录未运行。"));
		DWORD processId = 0;
		DWORD threadId = GetWindowThreadProcessId(window, &processId);

		m_sharedData->setWindow(window);

		std::string dllName = Apis::GetModuleDir() + "/Th10Hook.dll";
		DllInject::EnableDebugPrivilege();
		DllInject::Inject(processId, dllName);

		if (!m_sharedData->waitInit(3000))
			BASE_THROW(Exception(u8"Th10Hook初始化失败，详细信息请查看Th10Hook.log。"));

		//m_writeStatus = std::make_shared<Status>();
		//m_middleStatus = std::make_shared<Status>();
		//m_readStatus = std::make_shared<Status>();

		m_scene.split(6);

		m_controlThread = std::thread(&Th10Ai::controlProc, this);
		//m_readThread = std::thread(&Th10Ai::readProc, this);
		m_handleThread = std::thread(&Th10Ai::handleProc, this);
	}

	Th10Ai::~Th10Ai()
	{
		m_handleDone = true;
		if (m_handleThread.joinable())
			m_handleThread.join();

		m_readDone = true;
		m_sharedData->notifyUninit();
		if (m_readThread.joinable())
			m_readThread.join();

		m_controlDone = true;
		if (m_controlThread.joinable())
			m_controlThread.join();

		m_sharedMemory.destroy_ptr(m_sharedData);
	}

	bool Th10Ai::IsKeyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Th10Ai::controlProc()
	{
		try
		{
			std::cout << "请将焦点放在风神录窗口上，开始游戏，然后按A启动AI，按S停止AI，按D退出AI。" << std::endl;

			while (!m_controlDone)
			{
				if (IsKeyPressed('A'))
				{
					start();
				}
				else if (IsKeyPressed('S'))
				{
					stop();
				}
				else if (IsKeyPressed('D'))
				{
					break;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}

			stop();
			std::cout << "退出AI。" << std::endl;
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		//g_th10Hook.exit();
	}

	void Th10Ai::start()
	{
		if (!m_controlActive)
		{
			m_controlActive = true;
			m_readActive = true;
			m_handleActive = true;

			std::cout << "启动AI。" << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_controlActive)
		{
			m_controlActive = false;
			m_readActive = false;
			m_handleActive = false;

			std::cout << "停止AI。" << std::endl;
			std::cout << "决死总数：" << m_bombCount << std::endl;
		}
	}

	void Th10Ai::readProc()
	{
		try
		{
			while (!m_readDone)
			{
				if (m_readActive)
				{
					//m_status.update();
					//m_status.getPlayer().checkPrevMove(m_prevDir, m_prevSlow);
					//m_writeStatus->update();
					//if (!m_context.waitUpdate())
					//{
					//	m_handleDone = true;
					//	m_readDone = true;
					//	m_controlDone = true;
					//	break;
					//}
					//m_window->update(m_context.getStatus());

					//lock_guard<mutex> lock(m_statusMutex);
					//std::swap(m_writeStatus, m_middleStatus);
					//m_statusUpdated = true;
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(16));
				}
			}
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
			m_controlDone = true;
		}
	}

	void Th10Ai::handleProc()
	{
		try
		{
			while (!m_handleDone)
			{
				if (m_handleActive)
				{
					handle();
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(16));
				}
			}
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
			m_controlDone = true;
		}
	}

	void Th10Ai::handle()
	{
		if (!m_sharedData->waitUpdate())
		{
			m_handleDone = true;
			m_readDone = true;
			m_controlDone = true;
			return;
		}

		m_myWindow->update(m_sharedData->getStatus());

		m_status2.update(m_status1);
		m_status1.update(m_status);
		m_status.update(m_sharedData->getStatus());
		//m_status.getPlayer().checkPrevMove(m_prevDir, m_prevSlow);

		//if (m_statusUpdated)
		//{
		//	lock_guard<mutex> lock(m_statusMutex);
		//	std::swap(m_readStatus, m_middleStatus);
		//	m_statusUpdated = false;
		//}
		//else
		//{
		//	return;
		//}

		m_scene.clearAll();
		m_scene.splitEnemies(m_status.getEnemies());
		m_scene.splitBullets(m_status.getBullets());
		m_scene.splitLasers(m_status.getLasers());

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		m_sharedData->commit();
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_status.getPlayer().isColliding())
		{
			Clock clock;
			clock.update();
			if (clock.getTime() - m_bombTime > 1000)
			{
				m_bombTime = clock.getTime();

				int_t id = m_status.collide(m_status.getPlayer(), 0.0);
				m_status1.collide(m_status.getPlayer(), 1.0);
				m_status2.collide(m_status.getPlayer(), 2.0);
				m_status1.collide(m_status.getPlayer(), 1.0, id);
				m_status2.collide(m_status.getPlayer(), 2.0, id);

				m_sharedData->getAction().bomb = true;
				++m_bombCount;
				std::cout << "决死：" << m_bombCount << std::endl;
				return true;
			}
			else
			{
				m_sharedData->getAction().bomb = false;
				return false;
			}
		}
		else
		{
			m_sharedData->getAction().bomb = false;
			return false;
		}
	}

	// 处理对话
	bool Th10Ai::handleTalk()
	{
		if (m_status.isTalking())
		{
			m_sharedData->getAction().skip = true;
			return true;
		}
		else
		{
			m_sharedData->getAction().skip = false;
			return false;
		}
	}

	// 处理攻击
	bool Th10Ai::handleShoot()
	{
		if (m_status.hasEnemy())
		{
			m_sharedData->getAction().shoot = true;
			return true;
		}
		else
		{
			m_sharedData->getAction().shoot = false;
			return false;
		}
	}

	// 处理移动
	bool Th10Ai::handleMove()
	{
		if (!m_status.getPlayer().isNormalStatus())
			return false;

		boost::optional<Item> itemTarget = m_status.findItem();
		boost::optional<Enemy> enemyTarget = m_status.findEnemy();
		bool underEnemy = m_status.isUnderEnemy();

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		boost::optional<DIR> bestDir;
		boost::optional<bool> bestSlow;

		for (DIR dir : g_dirs)
		{
			Path path(m_status, m_scene, itemTarget, enemyTarget, underEnemy);
			Result result = path.find(dir);

			if (result.valid && path.m_bestScore > bestScore)
			{
				bestScore = path.m_bestScore;
				bestDir = path.m_dir;
				bestSlow = result.slow;
			}
		}

		if (bestDir.has_value() && bestSlow.has_value())
		{
			move(bestDir.value(), bestSlow.value());
		}
		else
		{
			move(DIR::HOLD, false);
			std::cout << "无路可走。" << std::endl;
		}

		return true;
	}

	void Th10Ai::move(DIR dir, bool slow)
	{
		if (slow)
			m_sharedData->getAction().slow = true;
		else
			m_sharedData->getAction().slow = false;

		switch (dir)
		{
		case DIR::HOLD:
			m_sharedData->getAction().left = false;
			m_sharedData->getAction().right = false;
			m_sharedData->getAction().up = false;
			m_sharedData->getAction().down = false;
			break;

		case DIR::LEFT:
			m_sharedData->getAction().left = true;
			m_sharedData->getAction().right = false;
			m_sharedData->getAction().up = false;
			m_sharedData->getAction().down = false;
			break;

		case DIR::RIGHT:
			m_sharedData->getAction().left = false;
			m_sharedData->getAction().right = true;
			m_sharedData->getAction().up = false;
			m_sharedData->getAction().down = false;
			break;

		case DIR::UP:
			m_sharedData->getAction().left = false;
			m_sharedData->getAction().right = false;
			m_sharedData->getAction().up = true;
			m_sharedData->getAction().down = false;
			break;

		case DIR::DOWN:
			m_sharedData->getAction().left = false;
			m_sharedData->getAction().right = false;
			m_sharedData->getAction().up = false;
			m_sharedData->getAction().down = true;
			break;

		case DIR::LEFTUP:
			m_sharedData->getAction().left = true;
			m_sharedData->getAction().right = false;
			m_sharedData->getAction().up = true;
			m_sharedData->getAction().down = false;
			break;

		case DIR::RIGHTUP:
			m_sharedData->getAction().left = false;
			m_sharedData->getAction().right = true;
			m_sharedData->getAction().up = true;
			m_sharedData->getAction().down = false;
			break;

		case DIR::LEFTDOWN:
			m_sharedData->getAction().left = true;
			m_sharedData->getAction().right = false;
			m_sharedData->getAction().up = false;
			m_sharedData->getAction().down = true;
			break;

		case DIR::RIGHTDOWN:
			m_sharedData->getAction().left = false;
			m_sharedData->getAction().right = true;
			m_sharedData->getAction().up = false;
			m_sharedData->getAction().down = true;
			break;
		}

		m_prevDir = dir;
		m_prevSlow = slow;
	}
}

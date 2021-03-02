#include "Th10Ai/Th10Ai.h"

#include <boost/optional.hpp>
#include <Base/Time.h>
#include <Base/Windows/Process.h>
#include <Base/Windows/Module.h>

#include "Th10Ai/DllInject.h"
#include "Th10Ai/Path.h"

namespace th
{
	Th10Ai::Th10Ai() :
		m_active(false),
		m_bombTime(0),
		m_bombCount(0)
	{
		HWND window = FindWindowW(L"BASE", nullptr);//L"搶曽晽恄榐丂乣 Mountain of Faith. ver 1.00a");
		if (window == nullptr)
			BASE_THROW(Exception(u8"东方风神录未运行。"));
		DWORD processId = 0;
		DWORD threadId = GetWindowThreadProcessId(window, &processId);

		m_sharedMemory = interprocess::managed_windows_shared_memory(
			interprocess::create_only, "Th10-SharedMemory", 8 * 1024 * 1024);
		m_sharedData = m_sharedMemory.construct<SharedData>("Th10-SharedData")();
		if (m_sharedData == nullptr)
			BASE_THROW(Exception(u8"Th10-SharedData名称已被使用。"));

		m_sharedData->setWindow(window);

		DllInject::EnableDebugPrivilege();
		Process target = Process::Open(PROCESS_ALL_ACCESS, FALSE, processId);
		std::string dllPath = Module().getDir() + "/Th10Hook.dll";
		DllInject::Inject(target, dllPath);

		if (!m_sharedData->waitInit(3000))
			BASE_THROW(Exception(u8"Th10Hook初始化失败，详细信息请查看Th10Hook.log。"));

		m_scene.split(6);
	}

	Th10Ai::~Th10Ai()
	{
		m_sharedData->notifyUninit();
		m_sharedMemory.destroy_ptr(m_sharedData);
	}

	bool IsKeyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Th10Ai::run()
	{
		std::cout << "请将焦点放在风神录窗口上，开始游戏，然后按A启动AI，按S停止AI，按D退出AI。" << std::endl;
		while (true)
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
				stop();
				break;
			}
			else if (!handle())
			{
				stop();
				break;
			}
		}
		std::cout << "退出AI。" << std::endl;
	}

	void Th10Ai::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "启动AI。" << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_active)
		{
			m_active = false;
			std::cout << "停止AI。" << std::endl;
			std::cout << "决死总数：" << m_bombCount << std::endl;
		}
	}

	bool Th10Ai::handle()
	{
		if (!m_active)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
			return true;
		}

		if (!m_sharedData->waitUpdate())
		{
			return false;
		}

		m_status2.update(m_status1);
		m_status1.update(m_status);
		m_status.update(m_sharedData->getStatus());

		m_scene.clearAll();
		m_scene.splitEnemies(m_status.getEnemies());
		m_scene.splitBullets(m_status.getBullets());
		m_scene.splitLasers(m_status.getLasers());

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		m_sharedData->commit();

		return true;
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_status.getPlayer().isColliding())
		{
			Time time = Time::Now();
			int64_t now = time.getMilliSeconds();
			if (now - m_bombTime > 1000)
			{
				m_bombTime = now;

				int_t id = m_status.collide(m_status.getPlayer(), 0.0);
				m_status1.collide(m_status.getPlayer(), 1.0);
				m_status1.collide(m_status.getPlayer(), 2.0);
				m_status1.collide(m_status.getPlayer(), 1.0, id);
				m_status1.collide(m_status.getPlayer(), 2.0, id);

				m_sharedData->getAction().bomb = true;
				m_bombCount += 1;
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

		for (DIR dir : DIRS)
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
	}
}

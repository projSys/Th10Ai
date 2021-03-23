#include "Th10Hook/Th10Ai.h"

#include <Base/Time.h>
#include <Base/Windows/Apis.h>

#include "Th10Hook/Path.h"

namespace th
{
	std::unique_ptr<Th10Ai> g_th10Ai;

	Th10Ai::Th10Ai(HWND window) :
		m_controlDone(false),
		m_handleDone(false),
		m_active(false),
		m_statusUpdated(false),
		m_bombTime(0),
		m_bombCount(0),
		m_findItemTime(0),
		m_inputUpdated(false),
		inputFrame(0),
		statusFrame(0),
		handleFrame(0)
	{
		SetForegroundWindow(window);

		RECT rect = {};
		GetWindowRect(window, &rect);
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int x = (screenWidth - (rect.right - rect.left)) / 2;
		int y = (screenHeight - (rect.bottom - rect.top)) / 2;
		SetWindowPos(window, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		char buf1[1024] = {};
		GetWindowTextA(window, buf1, 1023);
		std::wstring buf2 = Apis::MultiByteToWideChar(932, buf1, -1);	// Shift-JIS
		std::string buf3 = Apis::WideToAnsi(buf2);
		SetWindowTextA(window, buf3.c_str());

		//ULONG_PTR icon = GetClassLongPtrA(window, GCLP_HICON);
		HICON icon = LoadIconA(GetModuleHandleA(nullptr), "IDI_ICON3");
		SendMessageA(window, WM_SETICON, ICON_SMALL, (LPARAM)icon);

		m_writeStatus = std::make_unique<Status>();
		m_middleStatus = std::make_unique<Status>();
		m_readStatus = std::make_unique<Status>();

		m_scene.split(6);

		m_controlThread = std::thread(&Th10Ai::controlProc, this);
		m_handleThread = std::thread(&Th10Ai::handleProc, this);
	}

	Th10Ai::~Th10Ai()
	{
		m_handleDone = true;
		m_active = false;
		{
			std::unique_lock<std::mutex> lock(m_statusMutex);
			m_statusUpdated = true;
			m_statusCond.notify_one();
		}
		if (m_handleThread.joinable())
			m_handleThread.join();

		m_controlDone = true;
		if (m_controlThread.joinable())
			m_controlThread.join();
	}

	bool IsKeyPressed(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Th10Ai::controlProc()
	{
		std::cout << "Keep the focus on the game window, start the game, then press \'A\' to start AI, and press \'S\' to stop AI." << std::endl;
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
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
	}

	void Th10Ai::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "AI has started." << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_active)
		{
			m_active = false;
			std::cout << "AI has stopped." << std::endl;
			std::cout << "DeathBomb: " << m_bombCount << std::endl;
		}
	}

	void Th10Ai::handleProc()
	{
		while (!m_handleDone)
		{
			if (m_active)
			{
				handle();
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
		}
	}

	void Th10Ai::updateStatus()
	{
		if (!m_active)
			return;

		++statusFrame;

		m_writeStatus->update();

		std::unique_lock<std::mutex> lock(m_statusMutex);
		m_writeStatus.swap(m_middleStatus);
		m_statusUpdated = true;
		m_statusCond.notify_one();
	}

	bool Th10Ai::handle()
	{
		{
			std::unique_lock<std::mutex> lock(m_statusMutex);
			if (!m_statusUpdated)
				m_statusCond.wait(lock);
			else
				std::cout << "Processing is too slow." << std::endl;
			m_readStatus.swap(m_middleStatus);
			m_statusUpdated = false;
		}

		if (!m_active)
			return false;

		++handleFrame;
		m_readStatus->inputFrame = inputFrame;
		m_readStatus->statusFrame = statusFrame;
		m_readStatus->handleFrame = handleFrame;

		//m_status2.copy(m_status1);
		//m_status1.copy(m_status0);
		//m_status0.copy(*m_readStatus);

		//m_scene2.clearAll();
		//m_scene2.splitEnemies(m_status2.getEnemies());
		//m_scene2.splitBullets(m_status2.getBullets());
		//m_scene2.splitLasers(m_status2.getLasers());

		//m_scene1.clearAll();
		//m_scene1.splitEnemies(m_status1.getEnemies());
		//m_scene1.splitBullets(m_status1.getBullets());
		//m_scene1.splitLasers(m_status1.getLasers());

		m_scene.clearAll();
		m_scene.splitEnemies(m_readStatus->getEnemies());
		m_scene.splitBullets(m_readStatus->getBullets());
		m_scene.splitLasers(m_readStatus->getLasers());

		m_input.clear();

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		m_inputUpdated = true;

		return true;
	}

	void Th10Ai::commitInput(DWORD size, LPVOID data)
	{
		if (!m_active)
			return;

		++inputFrame;

		if (m_inputUpdated)
		{
			m_input.commitTo(size, data);
			m_inputUpdated = false;
		}
		else
		{
			std::cout << handleFrame << " Input is too slow." << std::endl;
		}
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_readStatus->getPlayer().isColliding())
		{
			int64_t now = Time::Now().getMilliSeconds();
			if (now - m_bombTime > 1000)
			{
				m_bombTime = now;

				//int_t id = m_status0.collide(m_status0.getPlayer(), 0);
				//m_status1.collide(m_status0.getPlayer(), 1);
				//m_status2.collide(m_status0.getPlayer(), 2);
				//m_status1.collide(m_status0.getPlayer(), 1, id);
				//m_status2.collide(m_status0.getPlayer(), 2, id);

				//RegionCollideResult rcr = m_scene.collideAll(m_status0.getPlayer(), 0);
				//std::cout << rcr.collided << std::endl;
				//RegionCollideResult rcr1 = m_scene1.collideAll(m_status0.getPlayer(), 1);
				//std::cout << rcr1.collided << std::endl;
				//RegionCollideResult rcr2 = m_scene2.collideAll(m_status0.getPlayer(), 2);
				//std::cout << rcr2.collided << std::endl;

				m_input.bomb();
				++m_bombCount;
				std::cout << handleFrame << " DeathBomb: " << m_bombCount << std::endl;
				return true;
			}
		}
		return false;
	}

	// 处理对话
	bool Th10Ai::handleTalk()
	{
		if (m_readStatus->isTalking())
		{
			m_input.skip();
			return true;
		}
		return false;
	}

	// 处理攻击
	bool Th10Ai::handleShoot()
	{
		if (m_readStatus->haveEnemies())
		{
			m_input.shoot();
			return true;
		}
		return false;
	}

	// 处理移动
	bool Th10Ai::handleMove()
	{
		if (!m_readStatus->getPlayer().isNormalStatus())
			return false;

		boost::optional<Item> itemTarget = findItem();
		boost::optional<Enemy> enemyTarget = findEnemy();
		bool underEnemy = m_readStatus->isUnderEnemy();

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		boost::optional<DIR> bestDir;
		boost::optional<bool> bestSlow;

		for (DIR dir : DIRS)
		{
			Path path(*m_readStatus, m_scene, itemTarget, enemyTarget, underEnemy);
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
			m_input.move(bestDir.value());
			if (bestSlow.value())
				m_input.slow();
		}
		else
		{
			std::cout << "No way to go." << std::endl;
		}

		return true;
	}

	// 查找道具
	boost::optional<Item> Th10Ai::findItem()
	{
		const Player& player = m_readStatus->getPlayer();
		const std::vector<Item>& items = m_readStatus->getItems();
		const std::vector<Enemy>& enemies = m_readStatus->getEnemies();

		boost::optional<Item> target;

		if (items.empty())
			return target;

		int64_t now = Time::Now().getMilliSeconds();
		// 拾取冷却中
		if (now - m_findItemTime < 3000)
			return target;

		// 自机高于1/4屏
		if (player.pos.y < Scene::SIZE.y / 4)
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		// 自机高于1/2屏，道具少于10个，敌人多于5个
		if ((player.pos.y < Scene::SIZE.y / 2) && (items.size() < 10) && (enemies.size() > 5))
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		float_t minDist = std::numeric_limits<float_t>::max();
		//float_t maxY = std::numeric_limits<float_t>::lowest();
		for (const Item& item : items)
		{
			if (!Scene::IsInScene(item.pos))
				continue;

			// 道具高于1/5屏
			if (item.pos.y < Scene::SIZE.y / 5)
				continue;

			// 道具不在自机1/4屏内
			float_t dy = std::abs(item.pos.y - player.pos.y);
			if (dy > Scene::SIZE.y / 4)
				continue;

			// 道具太靠近敌机
			bool tooClose = false;
			for (const Enemy& enemy : enemies)
			{
				if (item.distance(enemy) < 100)
				{
					tooClose = true;
					break;
				}
			}
			if (tooClose)
				continue;

			// 道具与自机距离最近
			float_t dist = item.distance(player);
			if (dist < minDist)
			{
				minDist = dist;
				target = item;
			}

			//if (item.y > maxY)
			//{
			//	maxY = item.y;
			//	target = item;
			//}
		}

		return target;
	}

	// 查找敌人
	boost::optional<Enemy> Th10Ai::findEnemy()
	{
		const Player& player = m_readStatus->getPlayer();
		const std::vector<Enemy>& enemies = m_readStatus->getEnemies();

		boost::optional<Enemy> target;

		if (enemies.empty())
			return target;

		// 自机高于1/4屏
		if (player.pos.y < Scene::SIZE.y / 4)
			return target;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (const Enemy& enemy : enemies)
		{
			if (!Scene::IsInScene(enemy.pos))
				continue;

			if (enemy.pos.y > player.pos.y)
				continue;

			// 与自机X轴距离最近
			float_t dx = std::abs(enemy.pos.x - player.pos.x);
			if (dx < minDist)
			{
				minDist = dx;
				target = enemy;
			}
		}

		return target;
	}
}

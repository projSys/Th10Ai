#include "Th10Ai/Th10Ai.h"

#include <Base/Exception/Catcher.h>

#include "Th10Ai/Path.h"

namespace th
{
	Th10Ai::Th10Ai(Th10AiListener* listener) :
		m_listener(listener),
		m_controlDone(false),
		m_handleDone(false),
		m_bombTime(0),
		m_bombCount(0),
		m_findItemTime(0)
	{
		m_th10Hook.launch(m_config);
		m_controlThread = std::thread(&Th10Ai::controlProc, this);
		m_handleThread = std::thread(&Th10Ai::handleProc, this);
	}

	Th10Ai::~Th10Ai()
	{
		m_handleDone = true;
		m_th10Hook.notifyExit();
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
		try
		{
			std::cout << "保持窗口焦点在风神录上，开始游戏，按A启动AI，按S停止AI。" << std::endl;
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
					std::this_thread::sleep_for(Time(16));
				}
			}
		}
		catch (...)
		{
			LOG_FATAL() << Catcher() << std::endl;
			Log::Flush();
			//ExitProcess(1);
		}
	}

	void Th10Ai::start()
	{
		if (!m_th10Hook.isActive())
		{
			m_th10Hook.setActive(true);
			std::cout << "AI启动。" << std::endl;
		}
	}

	void Th10Ai::stop()
	{
		if (m_th10Hook.isActive())
		{
			m_th10Hook.setActive(false);
			std::cout << "AI停止。" << std::endl;
			std::cout << "决死：" << m_bombCount << std::endl;
		}
	}

	void Th10Ai::focus()
	{
		m_th10Hook.focus();
	}

	void Th10Ai::handleProc()
	{
		try
		{
			while (!m_handleDone)
				handle();
		}
		catch (...)
		{
			LOG_FATAL() << Catcher() << std::endl;
			Log::Flush();
			//ExitProcess(1);
		}
	}

	bool Th10Ai::handle()
	{
		if (!m_th10Hook.isActive())
		{
			std::this_thread::sleep_for(Time(16));
			return false;
		}

		if (!m_th10Hook.waitUpdate())
		{
			m_handleDone = true;
			return false;
		}
		//if (!m_th10Hook.waitUpdate(Time(17)))
		//{
		//	std::cout << "错误：等待状态更新超时。" << std::endl;
		//	return false;
		//}
		//if (m_th10Hook.isExit())
		//{
		//	m_handleDone = true;
		//	return false;
		//}

		//Time t0 = Clock::Now();
		//Time diff = t0 - m_handleTime;
		//if (diff > Time(17))
		//	std::cout << diff << std::endl;
		//m_handleTime = t0;

		//static Time t0 = Clock::Now();
		//static int_t c0 = 0;
		//c0++;
		//if (Clock::Now() - t0 > Time(1000))
		//{
		//	std::cout << c0 << std::endl;
		//	t0 += Time(1000);
		//	c0 = 0;
		//}

		Time t1 = Clock::Now();

		m_status.copy(m_th10Hook.getReadableStatus());
		m_status.updateExtra();

		m_listener->onStatusUpdate(m_status);

		//m_status2.copy(m_status1);
		//m_status1.copy(m_status0);
		//m_status0.copy(*m_readableStatus);

		//m_scene2.clearAll();
		//m_scene2.splitEnemies(m_status2.getEnemies());
		//m_scene2.splitBullets(m_status2.getBullets());
		//m_scene2.splitLasers(m_status2.getLasers());

		//m_scene1.clearAll();
		//m_scene1.splitEnemies(m_status1.getEnemies());
		//m_scene1.splitBullets(m_status1.getBullets());
		//m_scene1.splitLasers(m_status1.getLasers());

		Time t2 = Clock::Now();

		m_scene.clearAll();
		m_scene.splitEnemies(m_status.getEnemies());
		m_scene.splitBullets(m_status.getBullets());
		m_scene.splitLasers(m_status.getLasers());

		Time t3 = Clock::Now();

		SharedInput& input = m_th10Hook.getWritableInput();
		input.clear();

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		Time t4 = Clock::Now();
		//if (t4 - t1 > Time(5))
		//	std::cout << t2 - t1 << ' ' << t3 - t2 << ' ' << t4 - t3 << std::endl;

		m_th10Hook.notifyInput();

		return true;
	}

	// 处理炸弹
	bool Th10Ai::handleBomb()
	{
		if (m_status.getPlayer().isColliding())
		{
			Time now = Clock::Now();
			if (now - m_bombTime > Time(1000))
			{
				m_bombTime = now;

				//const Player& player = m_readableStatus->getPlayer();
				//std::optional<Bullet> target = m_readableStatus->collide(player, 0);
				//m_status1.collide(player, 1);
				////m_status2.collide(player, 2);
				//if (target.has_value())
				//{
				//	m_status1.collide(player, 1, target.value().id);
				//	//m_status2.collide(player, 2, target.value().id);

				//	RegionCollideResult rcr = m_scene.collideAll(player, 0, target.value());
				//	std::cout << rcr.collided << std::endl;
				//	RegionCollideResult rcr1 = m_scene1.collideAll(player, 1, target.value());
				//	std::cout << rcr1.collided << std::endl;
				//	//RegionCollideResult rcr2 = m_scene2.collideAll(player, 2, target.value());
				//	//std::cout << rcr2.collided << std::endl;
				//}

				SharedInput& input = m_th10Hook.getWritableInput();
				input.bomb();
				++m_bombCount;
				std::cout << " 决死：" << m_bombCount << std::endl;
				return true;
			}
		}
		return false;
	}

	// 处理对话
	bool Th10Ai::handleTalk()
	{
		if (m_status.isTalking())
		{
			SharedInput& input = m_th10Hook.getWritableInput();
			input.skip();
			return true;
		}
		return false;
	}

	// 处理攻击
	bool Th10Ai::handleShoot()
	{
		if (m_status.hasEnemy())
		{
			SharedInput& input = m_th10Hook.getWritableInput();
			input.shoot();
			return true;
		}
		return false;
	}

	// 处理移动
	bool Th10Ai::handleMove()
	{
		if (!m_status.getPlayer().isNormalStatus())
			return false;

		std::optional<Item> itemTarget = findItem();
		std::optional<Enemy> enemyTarget = findEnemy();
		bool underEnemy = m_status.isUnderEnemy();
		bool slowFirst = (!itemTarget.has_value() && underEnemy);
		//bool slowFirst = false;

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		std::optional<Direction> bestDir;
		std::optional<bool> bestSlow;

		for (Direction dir : g_directions)
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
			SharedInput& input = m_th10Hook.getWritableInput();
			input.move(bestDir.value());
			if (bestSlow.value())
				input.slow();
		}
		else
		{
			std::cout << "无路可走。" << std::endl;
		}

		return true;
	}

	// 查找道具
	std::optional<Item> Th10Ai::findItem()
	{
		const Player& player = m_status.getPlayer();
		const std::vector<Item>& items = m_status.getItems();
		const std::vector<Enemy>& enemies = m_status.getEnemies();
		std::optional<Item> target;

		if (items.empty())
			return target;

		Time now = Clock::Now();
		// 检测冷却
		if (now - m_findItemTime < Time(2000))
			return target;

		// 自机高于1/4屏
		if (player.pos.y < Scene::SIZE.y / 4)
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		// 自机高于1/2屏，敌机多于5个
		if ((player.pos.y < Scene::SIZE.y / 2) && (enemies.size() > 5))
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		float_t minDist = std::numeric_limits<float_t>::max();
		//float_t maxY = std::numeric_limits<float_t>::lowest();
		for (const Item& item : items)
		{
			// 道具在屏幕外
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
				if (item.distance(enemy) < 150)
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

	// 查找敌机
	std::optional<Enemy> Th10Ai::findEnemy()
	{
		const Player& player = m_status.getPlayer();
		const std::vector<Enemy>& enemies = m_status.getEnemies();
		std::optional<Enemy> target;

		if (enemies.empty())
			return target;

		// 自机高于1/4屏
		if (player.pos.y < Scene::SIZE.y / 4)
			return target;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (const Enemy& enemy : enemies)
		{
			// 敌机在屏幕外
			if (!Scene::IsInScene(enemy.pos))
				continue;

			// 敌机在自机下面
			if (enemy.pos.y > player.pos.y)
				continue;

			// 敌机与自机X轴距离最近
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

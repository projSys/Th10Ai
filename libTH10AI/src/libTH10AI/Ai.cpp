#include "libTH10AI/Common.h"
#include "libTH10AI/Ai.h"

#include <thread>

#include "libTH10AI/HookThread.h"
#include "libTH10AI/Path.h"

namespace th
{
	// 在东方窗口线程运行
	Ai::Ai() :
		m_data(m_api),
		m_active(false),
		m_itemId(-1),
		m_enemyId(-1),
		m_bombCount(0),
		m_prevDir(DIR_HOLD),
		m_prevSlow(false)
	{
		m_scene.split(6);

		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);

		HWND window = GetConsoleWindow();
		HMENU menu = GetSystemMenu(window, FALSE);
		EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);
	}

	// 在东方窗口线程运行
	Ai::~Ai()
	{
		try
		{
			m_di8Hook.clear();
			m_di8Hook.commit();

			m_di8Hook.enable(false);
			m_d3d9Hook.enable(false);
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}
	}

	// 在钩子线程运行
	void Ai::run(HookThread& container)
	{
		std::cout << "请将焦点放在风神录窗口上，开始游戏，然后按A开启AI，按S停止AI，按D退出AI。" << std::endl;

		while (!container.isDone())
		{
			if (isKeyPressed('A'))
				start();
			else if (isKeyPressed('S'))
				stop();
			else if (isKeyPressed('D'))
				break;
			else if (isKeyPressed('P'))
				print();

			update();
		}

		std::cout << "退出AI。" << std::endl;
	}

	void Ai::notify()
	{
		m_active = false;
		m_d3d9Hook.notifyPresent();
	}

	bool Ai::isKeyPressed(int vKey) const
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Ai::print()
	{
		m_data.print();
	}

	void Ai::start()
	{
		if (!m_active)
		{
			m_di8Hook.clear();
			m_di8Hook.commit();

			m_d3d9Hook.enable(true);
			m_di8Hook.enable(true);

			m_active = true;
			std::cout << "开启AI。" << std::endl;
		}
	}

	void Ai::stop()
	{
		if (m_active)
		{
			m_di8Hook.clear();
			m_di8Hook.commit();

			m_di8Hook.enable(false);
			m_d3d9Hook.enable(false);

			m_active = false;
			std::cout << "停止AI。" << std::endl;
			std::cout << "决死总次数：" << m_bombCount << std::endl;
		}
	}

	void Ai::update()
	{
		if (!m_active)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
			return;
		}

		m_d3d9Hook.waitPresent();

		//static int_t fps = 0;
		//static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		//++fps;
		//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		//std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		//if (interval.count() >= 1000)
		//{
		//	std::cout << "fps: " << fps << std::endl;
		//	fps = 0;
		//	begin += std::chrono::milliseconds(1000);
		//}

		//std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();

		m_data.update();
		m_data.checkPrevMove(m_prevDir, m_prevSlow);

		//std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		//time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
		//std::cout << "e1: " << e1 << std::endl;

		m_scene.clearAll();
		m_scene.splitEnemies(m_data.getEnemies());
		m_scene.splitBullets(m_data.getBullets());
		m_scene.splitLasers(m_data.getLasers());

		//std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		//time_t e2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		//std::cout << "e2: " << e2 << std::endl;

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		//std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		//time_t e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
		//std::cout << "e3: " << e3 << std::endl;

		m_di8Hook.commit();
	}

	// 处理炸弹
	bool Ai::handleBomb()
	{
		if (m_data.isColliding())
		{
			m_di8Hook.keyPress(DIK_X);
			++m_bombCount;
			std::cout << "决死：" << m_bombCount << std::endl;
			return true;
		}
		else
		{
			m_di8Hook.keyRelease(DIK_X);
			return false;
		}
	}

	// 处理对话
	bool Ai::handleTalk()
	{
		if (m_data.isTalking())
		{
			m_di8Hook.keyPress(DIK_LCONTROL);
			return true;
		}
		else
		{
			m_di8Hook.keyRelease(DIK_LCONTROL);
			return false;
		}
	}

	// 处理攻击
	bool Ai::handleShoot()
	{
		if (m_data.hasEnemy())
		{
			m_di8Hook.keyPress(DIK_Z);
			return true;
		}
		else
		{
			m_di8Hook.keyRelease(DIK_Z);
			return false;
		}
	}

	// 处理移动
	bool Ai::handleMove()
	{
		if (!(m_data.isRebirthStatus() || m_data.isNormalStatus()))
			return false;

		m_itemId = m_data.findItem();
		m_enemyId = m_data.findEnemy();
		bool underEnemy = m_data.isUnderEnemy();

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		Direction bestDir = DIR_NONE;
		bool bestSlow = false;

		for (int_t i = DIR_HOLD; i < DIR_MAXCOUNT; ++i)
		{
			Path path(m_data, m_scene, m_itemId, m_enemyId);
			Reward reward = path.find(static_cast<Direction>(i), underEnemy);

			if (reward.valid && path.m_bestScore > bestScore)
			{
				bestScore = path.m_bestScore;
				bestDir = static_cast<Direction>(i);
				bestSlow = reward.slow;
			}
		}

		if (bestDir != DIR_NONE)
		{
			move(bestDir, bestSlow);
		}
		else
		{
			move(DIR_HOLD, false);
			std::cout << "无路可走。" << std::endl;
		}

		return true;
	}

	void Ai::move(Direction dir, bool slow)
	{
		if (slow)
			m_di8Hook.keyPress(DIK_LSHIFT);
		else
			m_di8Hook.keyRelease(DIK_LSHIFT);

		switch (dir)
		{
		case DIR_HOLD:
			m_di8Hook.keyRelease(DIK_UP);
			m_di8Hook.keyRelease(DIK_DOWN);
			m_di8Hook.keyRelease(DIK_LEFT);
			m_di8Hook.keyRelease(DIK_RIGHT);
			break;

		case DIR_UP:
			m_di8Hook.keyPress(DIK_UP);
			m_di8Hook.keyRelease(DIK_DOWN);
			m_di8Hook.keyRelease(DIK_LEFT);
			m_di8Hook.keyRelease(DIK_RIGHT);
			break;

		case DIR_DOWN:
			m_di8Hook.keyRelease(DIK_UP);
			m_di8Hook.keyPress(DIK_DOWN);
			m_di8Hook.keyRelease(DIK_LEFT);
			m_di8Hook.keyRelease(DIK_RIGHT);
			break;

		case DIR_LEFT:
			m_di8Hook.keyRelease(DIK_UP);
			m_di8Hook.keyRelease(DIK_DOWN);
			m_di8Hook.keyPress(DIK_LEFT);
			m_di8Hook.keyRelease(DIK_RIGHT);
			break;

		case DIR_RIGHT:
			m_di8Hook.keyRelease(DIK_UP);
			m_di8Hook.keyRelease(DIK_DOWN);
			m_di8Hook.keyRelease(DIK_LEFT);
			m_di8Hook.keyPress(DIK_RIGHT);
			break;

		case DIR_LEFTUP:
			m_di8Hook.keyPress(DIK_UP);
			m_di8Hook.keyRelease(DIK_DOWN);
			m_di8Hook.keyPress(DIK_LEFT);
			m_di8Hook.keyRelease(DIK_RIGHT);
			break;

		case DIR_RIGHTUP:
			m_di8Hook.keyPress(DIK_UP);
			m_di8Hook.keyRelease(DIK_DOWN);
			m_di8Hook.keyRelease(DIK_LEFT);
			m_di8Hook.keyPress(DIK_RIGHT);
			break;

		case DIR_LEFTDOWN:
			m_di8Hook.keyRelease(DIK_UP);
			m_di8Hook.keyPress(DIK_DOWN);
			m_di8Hook.keyPress(DIK_LEFT);
			m_di8Hook.keyRelease(DIK_RIGHT);
			break;

		case DIR_RIGHTDOWN:
			m_di8Hook.keyRelease(DIK_UP);
			m_di8Hook.keyPress(DIK_DOWN);
			m_di8Hook.keyRelease(DIK_LEFT);
			m_di8Hook.keyPress(DIK_RIGHT);
			break;
		}

		m_prevDir = dir;
		m_prevSlow = slow;
	}
}

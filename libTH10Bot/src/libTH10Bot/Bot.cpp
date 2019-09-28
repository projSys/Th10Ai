#include "libTH10Bot/Common.h"
#include "libTH10Bot/Bot.h"

#include <thread>

#include "libTH10Bot/HookThread.h"

namespace th
{
	// 在东方窗口线程运行
	Bot::Bot() :
		m_data(m_api),
		m_active(false),
		m_itemId(-1),
		m_enemyId(-1),
		m_bombCount(0),
		m_prevDir(DIR_HOLD),
		m_prevSlow(false),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_bestDir(DIR_NONE),
		m_bestSlow(false),
		m_count(0),
		m_limit(500)
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
	Bot::~Bot()
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
	void Bot::run(HookThread& container)
	{
		std::cout << "请将焦点放在风神录窗口上，开始游戏，然后按A开启Bot，按S停止Bot，按D退出Bot。" << std::endl;

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

		std::cout << "退出Bot。" << std::endl;
	}

	void Bot::notify()
	{
		m_active = false;
		m_d3d9Hook.notifyPresent();
	}

	bool Bot::isKeyPressed(int vKey) const
	{
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	void Bot::print()
	{
		m_data.print();
	}

	void Bot::start()
	{
		if (!m_active)
		{
			m_di8Hook.clear();
			m_di8Hook.commit();

			m_d3d9Hook.enable(true);
			m_di8Hook.enable(true);

			m_active = true;
			std::cout << "开启Bot。" << std::endl;
		}
	}

	void Bot::stop()
	{
		if (m_active)
		{
			m_di8Hook.clear();
			m_di8Hook.commit();

			m_di8Hook.enable(false);
			m_d3d9Hook.enable(false);

			m_active = false;
			std::cout << "停止Bot。" << std::endl;
			std::cout << "决死总次数：" << m_bombCount << std::endl;
		}
	}

	void Bot::update()
	{
		if (!m_active)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

		std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();

		m_data.update();
		m_data.checkPrevMove(m_prevDir, m_prevSlow);

		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
		//std::cout << "e1: " << e1 << std::endl;

		m_scene.clearAll();
		m_scene.splitEnemies(m_data.getEnemies());
		m_scene.splitBullets(m_data.getBullets());
		m_scene.splitLasers(m_data.getLasers());

		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		time_t e2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		//std::cout << "e2: " << e2 << std::endl;

		handleBomb();
		handleTalk();
		handleShoot();
		handleMove();

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		time_t e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
		//std::cout << "e3: " << e3 << std::endl;

		m_di8Hook.commit();
	}

	// 处理炸弹
	bool Bot::handleBomb()
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
	bool Bot::handleTalk()
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
	bool Bot::handleShoot()
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
	bool Bot::handleMove()
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
			Action action;
			action.fromPos = m_data.getPlayer().getPosition();
			action.fromDir = static_cast<Direction>(i);
			action.slowFirst = (m_itemId == -1 && underEnemy);
			action.frame = 1.0f;
			action.targetDir = static_cast<Direction>(i);

			m_bestScore = std::numeric_limits<float_t>::lowest();
			m_bestDir = DIR_NONE;
			m_bestSlow = false;
			m_count = 0;

			Reward reward = dfs(action);

			if (reward.valid && m_bestScore > bestScore)
			{
				bestScore = m_bestScore;
				bestDir = action.fromDir;
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

	Reward Bot::dfs(const Action& action)
	{
		Reward reward;
		reward.valid = false;
		reward.slow = false;
		reward.score = 0.0f;
		reward.size = 0;

		// 超过搜索节点限制
		++m_count;
		if (m_count >= m_limit)
			return reward;

		// 前进到下一个坐标
		Player temp = m_data.getPlayer();
		temp.setPosition(action.fromPos);
		temp.advance(action.fromDir, action.slowFirst);
		reward.slow = action.slowFirst;
		if (!Scene::IsInPlayerArea(temp.getPosition()) || m_scene.collideAll(temp, action.frame))
		{
			temp.setPosition(action.fromPos);
			temp.advance(action.fromDir, !action.slowFirst);
			reward.slow = !action.slowFirst;
			if (!Scene::IsInPlayerArea(temp.getPosition()) || m_scene.collideAll(temp, action.frame))
				return reward;
		}

		reward.valid = true;

		if (m_itemId != -1)
		{
			reward.score += calcCollectScore(temp);
		}
		else if (m_enemyId != -1)
		{
			reward.score += calcShootScore(temp);
		}
		else
		{
			reward.score += calcGobackScore(temp);
		}

		if (reward.score > m_bestScore)
		{
			m_bestScore = reward.score;
		}

		Mover mover(action.targetDir);
		reward.size = mover.getSize();
		while (mover.hasNext())
		{
			Direction dir = mover.next();

			Action nextAct;
			nextAct.fromPos = temp.getPosition();
			nextAct.fromDir = dir;
			nextAct.slowFirst = action.slowFirst;
			nextAct.frame = action.frame + 1.0f;
			nextAct.targetDir = action.targetDir;

			Reward nextRew = dfs(nextAct);

			if (m_count >= m_limit)
			{
				break;
			}

			if (!nextRew.valid)
			{
				reward.size -= 1;
				continue;
			}
		}
		// 没气了，当前节点也无效
		if (reward.size <= 0)
			reward.valid = false;

		return reward;
	}

	//NodeScore Bot::calcNodeScore(const Player& player, float_t frame)
	//{
	//	NodeScore score = { 0.0f, std::numeric_limits<float_t>::max(), DIR_NONE, 0 };

	//	for (const EntityView& view : m_focusBullets)
	//	{
	//		const Bullet& bullet = m_bullets[view.index];

	//		Bullet adv = bullet.advance(frame);
	//		float_t collideFrame = adv.willCollideWith(m_player);
	//		if (collideFrame > -1.0f)	// 已？/会碰撞
	//		{
	//			if (collideFrame < score.minCollideFrame)
	//			{
	//				score.minCollideFrame = collideFrame;
	//				score.minCollideDir = view.dir;
	//			}
	//			score.willCollideCount += 1;
	//		}
	//	}

	//	return score;
	//}

	// 拾取道具评分
	float_t Bot::calcCollectScore(const Player& player)
	{
		float_t score = 0.0f;

		if (m_itemId == -1)
			return score;

		const Item& item = m_data.getItems()[m_itemId];

		if (player.calcDistance(item.getPosition()) < 10.0f)
		{
			score += 300.0f;
		}
		else
		{
			float_t dx = std::abs(player.x - item.x);
			if (dx > SCENE_SIZE.width)
				dx = SCENE_SIZE.width;
			float_t dy = std::abs(player.y - item.y);
			if (dy > SCENE_SIZE.height)
				dy = SCENE_SIZE.height;

			score += 150.0f * (1.0f - dx / SCENE_SIZE.width);
			score += 150.0f * (1.0f - dy / SCENE_SIZE.height);
		}

		return score;
	}

	// 攻击敌人评分
	float_t Bot::calcShootScore(const Player& player)
	{
		float_t score = 0.0f;

		if (m_enemyId == -1)
			return score;

		const Enemy& enemy = m_data.getEnemies()[m_enemyId];

		float_t dx = std::abs(player.x - enemy.x);
		if (dx > SCENE_SIZE.width)
			dx = SCENE_SIZE.width;
		if (dx < 20.0f)
		{
			score += 150.0f;
		}
		else
		{
			// X轴距离越近得分越高
			score += 150.0f * (1.0f - dx / SCENE_SIZE.width);
		}

		float_t dy = std::abs(player.y - enemy.y);
		if (dy > SCENE_SIZE.height)
			dy = SCENE_SIZE.height;
		if (dy > SCENE_SIZE.height / 2.0f)
		{
			score += 150.0f;
		}
		else
		{
			// Y轴距离越远得分越高
			score += 150.0f * (dy / SCENE_SIZE.height);
		}

		return score;
	}

	float_t Bot::calcGobackScore(const Player& player)
	{
		float_t score = 0.0f;

		if (player.calcDistance(Player::INIT_POS) < 10.0f)
		{
			score += 30.0f;
		}
		else
		{
			float_t dx = std::abs(player.x - Player::INIT_POS.x);
			if (dx > SCENE_SIZE.width)
				dx = SCENE_SIZE.width;
			float_t dy = std::abs(player.y - Player::INIT_POS.y);
			if (dy > SCENE_SIZE.height)
				dy = SCENE_SIZE.height;

			score += 15.0f * (1.0f - dx / SCENE_SIZE.width);
			score += 15.0f * (1.0f - dy / SCENE_SIZE.height);
		}

		return score;
	}

	void Bot::move(Direction dir, bool slow)
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

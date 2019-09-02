#include "TH10Bot/Common.h"
#include "TH10Bot/Bot.h"

#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "TH10Bot/Mover.h"

#define PLAY 1

namespace th
{
	namespace sc = std::chrono;

	Bot::Bot() :
		m_active(false),
		m_process(Process::FindIdByName("th10.exe")),
		m_window(Window::FindByClassName("BASE")),
		m_graphCap(m_window, GC_D3D9FRAMESYNC),
		m_capturer(m_d3d),
		m_api(m_process),
		m_data(m_api),
		m_itemId(-1),
		m_enemyId(-1),
		m_bombCooldown(0),
		m_talkCooldown(0),
		m_collectCooldown(0),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_bestDir(DIR_NONE),
		m_bestSlow(false),
		m_count(0),
		m_limit(500)
	{
		m_scene.split(6);
	}

	Bot::~Bot()
	{
		try
		{
			m_input.keyRelease(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			m_input.keyRelease(KEY_LSHIFT);
			m_input.keyRelease(KEY_Z);
			m_input.keyRelease(KEY_X);
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
			std::cout << what << std::endl;
		}
	}

	bool Bot::isRunning()
	{
		return true;
	}

	void Bot::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "开启Bot。" << std::endl;
		}
	}

	void Bot::stop()
	{
		if (m_active)
		{
			m_input.keyRelease(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			m_input.keyRelease(KEY_LSHIFT);
			m_input.keyRelease(KEY_Z);
			m_input.keyRelease(KEY_X);

			m_active = false;
			std::cout << "停止Bot。" << std::endl;
		}
	}

	void Bot::quit()
	{

	}

	void Bot::run()
	{
		std::cout << "请将焦点放在风神录窗口上，开始游戏，然后按A开启Bot，按S停止Bot，按D退出。" << std::endl;

		while (true)
		{
			if (/*!threading.isRunning() ||*/ m_input.isKeyPressed(KEY_D))
			{
				quit();
				break;
			}
			else if (m_input.isKeyPressed(KEY_A))
			{
				start();
			}
			else if (m_input.isKeyPressed(KEY_S))
			{
				stop();
			}

			update();
		}
	}

	void Bot::update()
	{
		if (!m_active)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			return;
		}
#ifdef PLAY
		if (!m_frameSync.waitForPresent())
			std::cout << "跳帧。" << std::endl;
#else
		Rect rect = m_window.getClientRect();
		if (!m_capturer.capture(m_buffer, rect))
		{
			std::cout << "抓图失败：桌面没有变化导致超时，或者窗口位置超出桌面范围。" << std::endl;
			return;
		}
#endif
		static int_t fps = 0;
		static sc::steady_clock::time_point begin = sc::steady_clock::now();
		++fps;
		sc::steady_clock::time_point end = sc::steady_clock::now();
		sc::milliseconds interval = sc::duration_cast<sc::milliseconds>(end - begin);
		if (interval.count() >= 1000)
		{
			std::cout << "fps: " << fps << std::endl;
			fps = 0;
			begin += sc::milliseconds(1000);
		}

		m_clock.update();

		std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();

		m_data.update();

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
#ifdef PLAY
		handleBomb();
		if (!handleTalk())
			handleShoot();
		handleMove();

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		time_t e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
		time_t e4 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t0).count();
		if (e4 > 8)
			std::cout << "timeout: " << e1 << " " << e2 << " " << e3 << std::endl;
#else
		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		time_t e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
		time_t e4 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t0).count();
		//if (e4 > 10)
		//	std::cout << e1 << " " << e2 << " " << e3 << std::endl;

		cv::Scalar red(0, 0, 255);
		cv::Scalar green(0, 255, 0);
		cv::Scalar blue(255, 0, 0);
		cv::Scalar orange(0, 97, 255);
		cv::Scalar yellow(0, 255, 255);

		//m_scene.render(m_buffer.m_data, m_player);

		Pointi windowPos1 = Scene::ToWindowPos(m_player.getTopLeft());
		cv::Rect rect1(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
		cv::rectangle(m_buffer.m_data, rect1, green, -1);

		for (const Item& item : m_items)
		{
			Pointi windowPos = Scene::ToWindowPos(item.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(item.width), int_t(item.height));
			cv::rectangle(m_buffer.m_data, rect, blue, -1);
		}

		for (const Enemy& enemy : m_enemies)
		{
			Pointi windowPos = Scene::ToWindowPos(enemy.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(enemy.width), int_t(enemy.height));
			cv::rectangle(m_buffer.m_data, rect, red);
		}

		for (const Bullet& bullet : m_bullets)
		{
			Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
			cv::rectangle(m_buffer.m_data, rect, red, -1);

			// 显示垂足
			//FootPoint footPoint = bullet.getFootPoint(m_player.getPosition());
			//Pointi p1 = Scene::ToWindowPos(bullet.getPos());
			//Pointi p2 = Scene::ToWindowPos(Pointf(footPoint.x, footPoint.y));
			//Pointi p3 = Scene::ToWindowPos(m_player.getPos());
			//cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), orange);
			//cv::line(m_buffer.m_data, cv::Point(p2.x, p2.y), cv::Point(p3.x, p3.y), orange);

			//// 显示方向
			//if (view.dir == DIR_UP)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x, p1.y - 20), yellow);
			//else if (view.dir == DIR_DOWN)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x, p1.y + 20), yellow);
			//else if (view.dir == DIR_LEFT)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x - 20, p1.y), yellow);
			//else if (view.dir == DIR_RIGHT)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x + 20, p1.y), yellow);
			//else if (view.dir == DIR_UPLEFT)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x - 20, p1.y - 20), yellow);
			//else if (view.dir == DIR_UPRIGHT)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x + 20, p1.y - 20), yellow);
			//else if (view.dir == DIR_DOWNLEFT)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x - 20, p1.y + 20), yellow);
			//else if (view.dir == DIR_DOWNRIGHT)
			//	cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x + 20, p1.y + 20), yellow);
		}

		for (const Laser& laser : m_lasers)
		{
			LaserBox laserBox(laser);
			Pointi p1 = Scene::ToWindowPos(laserBox.topLeft);
			Pointi p2 = Scene::ToWindowPos(laserBox.topRight);
			Pointi p3 = Scene::ToWindowPos(laserBox.bottomRight);
			Pointi p4 = Scene::ToWindowPos(laserBox.bottomLeft);
			cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), red);
			cv::line(m_buffer.m_data, cv::Point(p2.x, p2.y), cv::Point(p3.x, p3.y), red);
			cv::line(m_buffer.m_data, cv::Point(p3.x, p3.y), cv::Point(p4.x, p4.y), red);
			cv::line(m_buffer.m_data, cv::Point(p4.x, p4.y), cv::Point(p1.x, p1.y), red);
		}

		cv::imshow("TH10", m_buffer.m_data);
		cv::waitKey(1);
#endif
	}

	// 处理炸弹
	bool Bot::handleBomb()
	{
		if (m_input.isKeyPressed(KEY_X))
			m_input.keyRelease(KEY_X);

		// 放了炸弹3秒后再检测
		if (m_clock.getTimestamp() - m_bombCooldown >= 3000)
		{
			if (m_data.isColliding())
			{
				m_input.keyPress(KEY_X);
				m_bombCooldown = m_clock.getTimestamp();
				return true;
			}
		}
		return false;
	}

	// 处理对话
	bool Bot::handleTalk()
	{
		if (m_data.isTalking())
		{
			// BOSS出现2秒后对话
			if (m_clock.getTimestamp() - m_talkCooldown >= 2000)
			{
				if (m_input.isKeyPressed(KEY_Z))
					m_input.keyRelease(KEY_Z);
				else
					m_input.keyPress(KEY_Z);
				return true;
			}
		}
		else
		{
			m_talkCooldown = m_clock.getTimestamp();
		}
		return false;
	}

	// 处理攻击
	bool Bot::handleShoot()
	{
		if (m_data.hasEnemy())
		{
			m_input.keyPress(KEY_Z);
			return true;
		}
		else
		{
			m_input.keyRelease(KEY_Z);
			return false;
		}
	}

	// 处理移动
	bool Bot::handleMove()
	{
		if (!(m_data.isRebirthStatus() || m_data.isNormalStatus()))
			return false;

		m_itemId = findItem();
		m_enemyId = findEnemy();

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

	// 查找道具
	int_t Bot::findItem()
	{
		int_t id = -1;

		if (m_data.getItems().empty())
			return id;

		// 拾取冷却中
		if (m_clock.getTimestamp() - m_collectCooldown < 3000)
			return id;

		// 自机高于1/4屏
		if (m_data.getPlayer().y < SCENE_SIZE.height / 4.0f)
		{
			// 进入冷却
			m_collectCooldown = m_clock.getTimestamp();
			return id;
		}

		// 自机高于1/2屏，道具少于10个，敌人多于3个
		if (m_data.getPlayer().y < SCENE_SIZE.height / 2.0f && m_data.getItems().size() < 10 && m_data.getEnemies().size() > 3)
		{
			// 进入冷却
			m_collectCooldown = m_clock.getTimestamp();
			return id;
		}

		float_t minDist = std::numeric_limits<float_t>::max();
		for (uint_t i = 0; i < m_data.getItems().size(); ++i)
		{
			const Item& item = m_data.getItems()[i];

			// 道具高于1/5屏
			if (item.y < SCENE_SIZE.height / 5.0f)
				continue;

			// 道具不在自机1/3屏内
			float_t dy = std::abs(item.y - m_data.getPlayer().y);
			if (dy > SCENE_SIZE.height / 3.0f)
				continue;

			bool tooClose = false;
			for (const Enemy& enemy : m_data.getEnemies())
			{
				if (item.calcDistance(enemy.getPosition()) < 100.0f)
				{
					tooClose = true;
					break;
				}
			}
			if (tooClose)
				continue;

			// 道具与自机距离最近
			float_t dist = item.calcDistance(m_data.getPlayer().getPosition());
			if (dist < minDist)
			{
				minDist = dist;
				id = i;
			}
		}

		return id;
	}

	// 查找敌人
	int_t Bot::findEnemy()
	{
		int_t id = -1;

		if (m_data.getEnemies().empty())
			return id;

		// 自机高于1/4屏
		if (m_data.getPlayer().y < SCENE_SIZE.height / 4.0f)
			return id;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (uint_t i = 0; i < m_data.getEnemies().size(); ++i)
		{
			const Enemy& enemy = m_data.getEnemies()[i];

			if (enemy.y > m_data.getPlayer().y)
				continue;

			// 与自机X轴距离最近
			float_t dx = std::abs(enemy.x - m_data.getPlayer().x);
			if (dx < minDist)
			{
				minDist = dx;
				id = i;
			}
		}

		return id;
	}

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
			m_input.keyPress(KEY_LSHIFT);
		else
			m_input.keyRelease(KEY_LSHIFT);

		switch (dir)
		{
		case DIR_HOLD:
			m_input.keyRelease(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			break;

		case DIR_UP:
			m_input.keyPress(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			break;

		case DIR_DOWN:
			m_input.keyRelease(KEY_UP);
			m_input.keyPress(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			break;

		case DIR_LEFT:
			m_input.keyRelease(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyPress(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			break;

		case DIR_RIGHT:
			m_input.keyRelease(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyPress(KEY_RIGHT);
			break;

		case DIR_UPLEFT:
			m_input.keyPress(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyPress(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			break;

		case DIR_UPRIGHT:
			m_input.keyPress(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyPress(KEY_RIGHT);
			break;

		case DIR_DOWNLEFT:
			m_input.keyRelease(KEY_UP);
			m_input.keyPress(KEY_DOWN);
			m_input.keyPress(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			break;

		case DIR_DOWNRIGHT:
			m_input.keyRelease(KEY_UP);
			m_input.keyPress(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyPress(KEY_RIGHT);
			break;
		}
	}
}

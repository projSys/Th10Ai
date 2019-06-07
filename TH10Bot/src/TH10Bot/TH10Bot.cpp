#include "TH10Bot/Common.h"
#include "TH10Bot/TH10Bot.h"

#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "TH10Bot/Mover.h"

#define PLAY 1

namespace th
{
	TH10Bot::TH10Bot() :
		m_process(Process::FindIdByName("th10.exe")),
		m_window(Window::FindByClassName("BASE")),
		m_graphCap(m_process, GC_D3D9FRAMESYNC | GC_DI8HOOK),
		m_reader(m_process),
		m_active(false),
		m_pause(false),
		m_itemId(-1),
		m_enemyId(-1),
		m_bombCooldown(0),
		m_talkCooldown(0),
		m_collectCooldown(0),
		m_bestScore(std::numeric_limits<float_t>::lowest()),
		m_bestDir(DIR_NONE),
		m_bestSlow(false),
		m_count(0),
		m_limit(1000)
	{
		m_items.reserve(200);
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);

		m_scene.split(6);
	}

	TH10Bot::~TH10Bot()
	{
		try
		{
			m_input.clear();
			m_input.update();
		}
		catch (...)
		{
		}
	}

	bool TH10Bot::isKeyPressed(int vkey) const
	{
		return (GetAsyncKeyState(vkey) & 0x8000) != 0;
	}

	void TH10Bot::run()
	{
		std::cout << "�뽫������ڷ���¼�����ϣ���ʼ��Ϸ��Ȼ��A����Bot����SֹͣBot����D�˳���" << std::endl;

		//int_t fps = 0;
		//std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
		while (true)
		{
			if (isKeyPressed('A'))
				start();
			if (isKeyPressed('S'))
				stop();
			if (isKeyPressed('D'))
				break;

			update();

			//++fps;
			//std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
			//time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
			//if (e1 >= 1000)
			//{
			//	std::cout << fps << std::endl;
			//	fps = 0;
			//	t0 += std::chrono::milliseconds(1000);
			//}
		}
	}

	void TH10Bot::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "����Bot��" << std::endl;
		}
	}

	void TH10Bot::stop()
	{
		if (m_active)
		{
			m_input.clear();
			m_input.update();
			m_active = false;
			std::cout << "ֹͣBot��" << std::endl;
		}
	}

	void TH10Bot::update()
	{
		if (!m_active)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(17));
			return;
		}
#if PLAY
		if (!m_frameSync.waitForPresent())
		{
			std::cout << "�ȴ�֡ͬ����ʱ��" << std::endl;
			return;
		}
#else
		Rect rect = m_window.getClientRect();
		if (!m_capturer.capture(m_buffer, rect))
		{
			std::cout << "ץͼʧ�ܣ�����û�б仯���³�ʱ�����ߴ���λ�ó������淶Χ��" << std::endl;
			return;
		}
#endif
		m_clock.update();

		std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();

		m_reader.getPlayer(m_player);
		m_reader.getItems(m_items);
		m_reader.getEnemies(m_enemies);
		m_reader.getBullets(m_bullets);
		m_reader.getLasers(m_lasers);

		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
		//std::cout << "e1: " << e1 << std::endl;

		m_scene.clearAll();
		m_scene.splitEnemies(m_enemies);
		m_scene.splitBullets(m_bullets);
		m_scene.splitLasers(m_lasers);

		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		time_t e2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		//std::cout << "e2: " << e2 << std::endl;
#if PLAY
		handleBomb();
		if (!handleTalk())
			handleShoot();
		handleMove();

		m_input.update();

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		time_t e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
		time_t e4 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t0).count();
		if (e4 > 10)
			std::cout << e1 << " " << e2 << " " << e3 << std::endl;
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

		m_scene.renderTo(m_buffer.m_data, m_player);

		Pointi windowPos1 = Scene::ToWindowPos(m_player.getTopLeft());
		cv::Rect rect1(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
		cv::rectangle(m_buffer.m_data, rect1, green, -1);
		//Pointi windowPos11 = Scene::ToWindowPos(m_player.getPos());
		//cv::Point center11(windowPos11.x, windowPos11.y);
		//cv::circle(m_buffer.m_data, center11, int_t(100.0f), green);
		/*
		for (const Item& item : m_items)
		{
			Pointi windowPos = Scene::ToWindowPos(item.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(item.width), int_t(item.height));
			cv::rectangle(m_buffer.m_data, rect, blue, -1);
		}

		//for (const EntityView& view : m_focusEnemies)
		//{
		//	const Enemy& enemy = m_enemies[view.index];
		for (const Enemy& enemy : m_enemies)
		{
			Pointi windowPos = Scene::ToWindowPos(enemy.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(enemy.width), int_t(enemy.height));
			cv::rectangle(m_buffer.m_data, rect, red);
		}

		for (const EntityView& view : m_focusBullets)
		{
			const Bullet& bullet = m_bullets[view.index];
		//for (const Bullet& bullet : m_bullets)
		//{
			Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
			cv::rectangle(m_buffer.m_data, rect, red, -1);

			// ��ʾ����
			//FootPoint footPoint = bullet.getFootPoint(m_player.getPos());
			//Pointi p1 = Scene::ToWindowPos(bullet.getPos());
			//Pointi p2 = Scene::ToWindowPos(Pointf(footPoint.x, footPoint.y));
			//Pointi p3 = Scene::ToWindowPos(m_player.getPos());
			//cv::line(m_buffer.m_data, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), orange);
			//cv::line(m_buffer.m_data, cv::Point(p2.x, p2.y), cv::Point(p3.x, p3.y), orange);

			//// ��ʾ����
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

		for (const EntityView& view : m_focusLasers)
		{
			const Laser& laser = m_lasers[view.index];
		//for (const Laser& laser : m_lasers)
		//{
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
		*/
		cv::imshow("TH10", m_buffer.m_data);
		cv::waitKey(1);
#endif
	}

	// ����ը��
	bool TH10Bot::handleBomb()
	{
		if (m_input.isKeyPressed(KEY_X))
			m_input.keyRelease(KEY_X);

		// ����ը��3����ټ��
		if (m_clock.getTimestamp() - m_bombCooldown >= 3000)
		{
			if (m_player.isColliding())
			{
				m_input.keyPress(KEY_X);
				m_bombCooldown = m_clock.getTimestamp();
				return true;
			}
		}
		return false;
	}

	// ����Ի�
	bool TH10Bot::handleTalk()
	{
		if (m_enemies.size() <= 1 && m_bullets.empty() && m_lasers.empty())
		{
			// BOSS����2���Ի�
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

	// ������
	bool TH10Bot::handleShoot()
	{
		if (!m_enemies.empty())
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

	// �����ƶ�
	bool TH10Bot::handleMove()
	{
		if (!(m_player.isReborn() || m_player.isNormal()))
			return false;

		m_itemId = findItem();
		m_enemyId = findEnemy();

		bool underEnemy = isUnderEnemy();

		float_t bestScore = std::numeric_limits<float_t>::lowest();
		Direction bestDir = DIR_NONE;
		bool bestSlow = false;

		for (int_t i = DIR_HOLD; i < DIR_MAXCOUNT; ++i)
		{
			Action action;
			action.fromPos = m_player.getPos();
			action.fromDir = static_cast<Direction>(i);
			action.slowFirst = (m_itemId == -1 && underEnemy);
			action.frame = 1.0f;
			action.targetDir = static_cast<Direction>(i);

			m_bestScore = std::numeric_limits<float_t>::lowest();
			m_bestDir = DIR_NONE;
			m_bestSlow = false;
			m_count = 0;

			Result result = dfs(action);

			if (result.valid && m_bestScore > bestScore)
			{
				bestScore = m_bestScore;
				bestDir = action.fromDir;
				bestSlow = result.slow;
			}
		}

		if (bestDir != DIR_NONE)
		{
			move(bestDir, bestSlow);
		}
		else
		{
			move(DIR_HOLD, false);
			std::cout << "��·���ߡ�" << std::endl;
		}

		return true;
	}

	Result TH10Bot::dfs(const Action& action)
	{
		Result result;
		result.valid = false;
		result.slow = false;
		result.score = 0.0f;
		result.size = 0;

		// ���������ڵ�����
		++m_count;
		if (m_count >= m_limit)
			return result;

		// ǰ������һ������
		Player temp = m_player;
		temp.setPos(action.fromPos);
		temp.advance(action.fromDir, action.slowFirst);
		result.slow = action.slowFirst;
		if (!Scene::IsInPlayerArea(temp.getPos()) || m_scene.collideAll(temp, action.frame))
		{
			temp.setPos(action.fromPos);
			temp.advance(action.fromDir, !action.slowFirst);
			result.slow = !action.slowFirst;
			if (!Scene::IsInPlayerArea(temp.getPos()) || m_scene.collideAll(temp, action.frame))
				return result;
		}

		result.valid = true;

		if (m_itemId != -1)
		{
			result.score += getCollectItemScore(temp);
		}
		else if (m_enemyId != -1)
		{
			result.score += getShootEnemyScore(temp);
		}
		else
		{
			result.score += getGobackScore(temp);
		}

		if (result.score > m_bestScore)
		{
			m_bestScore = result.score;
		}

		Mover mover(action.targetDir);
		result.size = mover.getSize();
		while (mover.hasNext())
		{
			Direction dir = mover.next();

			Action nextAct;
			nextAct.fromPos = temp.getPos();
			nextAct.fromDir = dir;
			nextAct.slowFirst = action.slowFirst;
			nextAct.frame = action.frame + 1.0f;
			nextAct.targetDir = action.targetDir;

			Result nextRes = dfs(nextAct);

			if (m_count >= m_limit)
			{
				break;
			}

			if (!nextRes.valid)
			{
				result.size -= 1;
				continue;
			}
		}
		// û���ˣ���ǰ�ڵ�Ҳ��Ч
		if (result.size <= 0)
			result.valid = false;

		return result;
	}

	bool TH10Bot::isUnderEnemy()
	{
		bool underEnemy = false;

		for (const Enemy& enemy : m_enemies)
		{
			if (std::abs(m_player.x - enemy.x) < 20.0f
				&& m_player.y > enemy.y)
			{
				underEnemy = true;
				break;
			}
		}

		return underEnemy;
	}

	//NodeScore TH10Bot::getNodeScore(const Player& player, float_t frame)
	//{
	//	NodeScore score = { 0.0f, std::numeric_limits<float_t>::max(), DIR_NONE, 0 };

	//	for (const EntityView& view : m_focusBullets)
	//	{
	//		const Bullet& bullet = m_bullets[view.index];

	//		Bullet adv = bullet.advance(frame);
	//		float_t collideFrame = adv.willCollideWith(m_player);
	//		if (collideFrame > -1.0f)	// �ѣ�/����ײ
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

	// ���ҵ���
	int_t TH10Bot::findItem()
	{
		int_t id = -1;

		if (m_items.empty())
			return id;

		// ʰȡ��ȴ��
		if (m_clock.getTimestamp() - m_collectCooldown < 4000)
			return id;

		// �Ի�����1/2������������10�������˶���2��
		if (m_player.y < SCENE_SIZE.height / 2.0f && m_items.size() < 10 && m_enemies.size() > 2)
		{
			// ������ȴ
			m_collectCooldown = m_clock.getTimestamp();
			return id;
		}

		// �Ի�����1/4��
		if (m_player.y < SCENE_SIZE.height / 4.0f)
		{
			// ������ȴ
			m_collectCooldown = m_clock.getTimestamp();
			return id;
		}

		float_t minDist = std::numeric_limits<float_t>::max();
		for (uint_t i = 0; i < m_items.size(); ++i)
		{
			const Item& item = m_items[i];

			// ���߸���1/5��
			if (item.y < SCENE_SIZE.height / 5.0f)
				continue;

			// ���߲����Ի�1/2����
			float_t dy = std::abs(item.y - m_player.y);
			if (dy > SCENE_SIZE.height / 3.0f)
				continue;

			// �������Ի��������
			float_t dist = item.getDist(m_player.getPos());
			if (dist < minDist)
			{
				minDist = dist;
				id = i;
			}
		}

		return id;
	}

	// ���ҵ���
	int_t TH10Bot::findEnemy()
	{
		int_t id = -1;

		if (m_player.y < SCENE_SIZE.height / 4.0f)
			return id;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (uint_t i = 0; i < m_enemies.size(); ++i)
		{
			const Enemy& enemy = m_enemies[i];

			if (enemy.y > m_player.y)
				continue;

			// ���Ի�X��������
			float_t dx = std::abs(enemy.x - m_player.x);
			if (dx < minDist)
			{
				minDist = dx;
				id = i;
			}
		}

		return id;
	}

	// ʰȡ��������
	float_t TH10Bot::getCollectItemScore(const Player& player)
	{
		float_t score = 0.0f;

		if (m_itemId == -1)
			return score;

		const Item& item = m_items[m_itemId];

		if (player.getDist(item.getPos()) < 10.0f)
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

	// ������������
	float_t TH10Bot::getShootEnemyScore(const Player& player)
	{
		float_t score = 0.0f;

		if (m_enemyId == -1)
			return score;

		const Enemy& enemy = m_enemies[m_enemyId];

		float_t dx = std::abs(player.x - enemy.x);
		if (dx > SCENE_SIZE.width)
			dx = SCENE_SIZE.width;
		if (dx < 20.0f)
		{
			score += 150.0f;
		}
		else
		{
			// X�����Խ���÷�Խ��
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
			// Y�����ԽԶ�÷�Խ��
			score += 150.0f * (dy / SCENE_SIZE.height);
		}

		return score;
	}

	float_t TH10Bot::getGobackScore(const Player& player)
	{
		float_t score = 0.0f;

		if (player.getDist(Player::INIT_POS) < 10.0f)
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

	void TH10Bot::move(Direction dir, bool slow)
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

	void TH10Bot::pause()
	{
		if (!m_pause)
		{
			m_input.keyRelease(KEY_LSHIFT);
			m_input.keyRelease(KEY_UP);
			m_input.keyRelease(KEY_DOWN);
			m_input.keyRelease(KEY_LEFT);
			m_input.keyRelease(KEY_RIGHT);
			m_input.keyRelease(KEY_Z);
			m_input.keyRelease(KEY_X);
			m_input.keyPress(KEY_ESC);
			std::this_thread::sleep_for(std::chrono::milliseconds(17));
			m_input.keyRelease(KEY_ESC);
			m_pause = true;
		}
	}

	void TH10Bot::resume()
	{
		if (m_pause)
		{
			m_input.keyPress(KEY_ESC);
			std::this_thread::sleep_for(std::chrono::milliseconds(17));
			m_input.keyRelease(KEY_ESC);
			m_pause = false;
		}
	}
}

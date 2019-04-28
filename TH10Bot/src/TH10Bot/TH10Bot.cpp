#include "TH10Bot/Common.h"
#include "TH10Bot/TH10Bot.h"

#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "TH10Bot/MyMath.h"
#include "TH10Bot/Mover.h"

#define PLAY 1

namespace th
{
	TH10Bot::TH10Bot() :
		m_process(Process::FindByName("th10.exe")),
		m_window(Window::FindByClassName("BASE")),
		m_graphCap(m_process, GHT_D3D9FRAMESYNC),
		m_reader(m_process),
		m_active(false),
		m_count(0),
		m_itemId(-1),
		m_enemyId(-1),
		m_bombCooldown(0),
		m_talkCooldown(0),
		m_shootCooldown(0),
		m_collectCooldown(0),
		m_draw(false)
	{
		m_items.reserve(200);
		m_enemies.reserve(200);
		m_bullets.reserve(1000);
		m_lasers.reserve(500);

		m_focusBullets.reserve(500);
		m_focusLasers.reserve(500);

		m_path.reserve(200);
		m_buffer = cv::Mat(480, 640, CV_8UC3, cv::Scalar(255, 255, 255));
		memset(m_mask, 0, sizeof(m_mask));

		srand((unsigned int)time(nullptr));
	}

	TH10Bot::~TH10Bot()
	{
		try
		{
			m_input.keyRelease(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			m_input.keyRelease(VK_SHIFT);
			m_input.keyRelease('Z');
			m_input.keyRelease('X');
		}
		catch (...)
		{
		}
	}

	void TH10Bot::run()
	{
		//int_t fps = 0;
		//std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
		while (true)
		{
			if (m_input.isKeyPressed('A'))
				start();
			if (m_input.isKeyPressed('S'))
				stop();
			if (m_input.isKeyPressed('D'))
				break;
			if (m_input.isKeyPressed(VK_LBUTTON))
				draw();

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

	void TH10Bot::draw()
	{
		m_draw = true;
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
			m_active = false;
			m_input.keyRelease(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			m_input.keyRelease(VK_SHIFT);
			m_input.keyRelease('Z');
			m_input.keyRelease('X');
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
		if (!m_sync.waitForPresent())
		{
			std::cout << "�ȴ�֡ͬ����ʱ��" << std::endl;
			return;
		}
#else
		Rect rect = m_window.getClientRect();
		if (!m_capturer.capture(m_image, rect))
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

		// �ü���Ļ
		m_focusBullets.clear();
		for (uint_t i = 0; i < m_bullets.size(); ++i)
		{
			const Bullet& bullet = m_bullets[i];

			float_t dist = bullet.getDist(m_player.getPos());
			FootPoint footPoint = bullet.getFootPoint(m_player.getPos());
			if (dist < 100.0f	// �ڸ�����
				|| (m_player.getDist(Pointf(footPoint.x, footPoint.y)) < 100.0f
					&& footPoint.frame >= 0.0f && footPoint.frame <= 180.0f))	// 3���ڿ�����ײ��
			{
				EntityView view;
				view.index = i;
				view.dir = bullet.getDir();
				m_focusBullets.push_back(view);
			}
		}
		//m_focusLasers.clear();
		//for (uint_t i = 0; i < m_lasers.size(); ++i)
		//{
		//	const Laser& laser = m_lasers[i];

		//	EntityView view;
		//	view.index = i;
		//	view.dist = laser.distance(m_player);
		//	view.footFrame = laser.footFrame(m_player.getPos());
		//	view.footPoint = laser.footPoint(view.footFrame);
		//	view.angle = laser.angle(m_player);
		//	view.dir = laser.direction();
		//	if (view.dist < 100.0f	// �ڸ�����
		//		|| (m_player.getPos().distance(view.footPoint) < 100.0f && view.angle > 0.0f && view.angle < 90.0f))	// ����������ײ��
		//	{
		//		m_focusLasers.push_back(view);
		//	}
		//}

		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		time_t e2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		//std::cout << "e2: " << e2 << std::endl;
#if PLAY
		handleBomb();
		if (!handleTalk())
			handleShoot();
		handleMove();

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		time_t e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
		time_t e4 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t0).count();
		if (e4 > 10)
			std::cout << e1 << " " << e2 << " " << e3 << std::endl;
#else
		cv::Scalar red(0, 0, 255);
		cv::Scalar green(0, 255, 0);
		cv::Scalar blue(255, 0, 0);
		cv::Scalar orange(0, 97, 255);
		cv::Scalar yellow(0, 255, 255);

		Pointi windowPos1 = Scene::ToWindowPos(m_player.getTopLeft());
		cv::Rect rect1(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
		cv::rectangle(m_image.m_data, rect1, green, -1);
		//Pointi windowPos11 = Scene::ToWindowPos(m_player.getPos());
		//cv::Point center11(windowPos11.x, windowPos11.y);
		//cv::circle(m_image.m_data, center11, int_t(100.0f), green);

		for (const Item& item : m_items)
		{
			Pointi windowPos = Scene::ToWindowPos(item.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(item.width), int_t(item.height));
			cv::rectangle(m_image.m_data, rect, blue, -1);
		}

		for (const Enemy& enemy : m_enemies)
		{
			Pointi windowPos = Scene::ToWindowPos(enemy.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(enemy.width), int_t(enemy.height));
			cv::rectangle(m_image.m_data, rect, red);
		}

		//for (const EntityView& view : m_focusBullets)
		//{
		//	const Bullet& bullet = m_bullets[view.index];
		for (const Bullet& bullet : m_bullets)
		{
			Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
			cv::rectangle(m_image.m_data, rect, red, -1);

			// ��ʾ����
			//FootPoint footPoint = bullet.getFootPoint(m_player.getPos());
			//Pointi p1 = Scene::ToWindowPos(bullet.getPos());
			//Pointi p2 = Scene::ToWindowPos(Pointf(footPoint.x, footPoint.y));
			//Pointi p3 = Scene::ToWindowPos(m_player.getPos());
			//cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), orange);
			//cv::line(m_image.m_data, cv::Point(p2.x, p2.y), cv::Point(p3.x, p3.y), orange);

			//// ��ʾ����
			//if (view.dir == DIR_UP)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x, p1.y - 20), yellow);
			//else if (view.dir == DIR_DOWN)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x, p1.y + 20), yellow);
			//else if (view.dir == DIR_LEFT)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x - 20, p1.y), yellow);
			//else if (view.dir == DIR_RIGHT)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x + 20, p1.y), yellow);
			//else if (view.dir == DIR_UPLEFT)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x - 20, p1.y - 20), yellow);
			//else if (view.dir == DIR_UPRIGHT)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x + 20, p1.y - 20), yellow);
			//else if (view.dir == DIR_DOWNLEFT)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x - 20, p1.y + 20), yellow);
			//else if (view.dir == DIR_DOWNRIGHT)
			//	cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p1.x + 20, p1.y + 20), yellow);
		}

		//for (const EntityView& view : m_focusLasers)
		//{
		//	const Laser& laser = m_lasers[view.index];
		for (const Laser& laser : m_lasers)
		{
			SATBox laserBox(laser);
			Pointi p1 = Scene::ToWindowPos(laserBox.topLeft);
			Pointi p2 = Scene::ToWindowPos(laserBox.topRight);
			Pointi p3 = Scene::ToWindowPos(laserBox.bottomRight);
			Pointi p4 = Scene::ToWindowPos(laserBox.bottomLeft);
			cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), red);
			cv::line(m_image.m_data, cv::Point(p2.x, p2.y), cv::Point(p3.x, p3.y), red);
			cv::line(m_image.m_data, cv::Point(p3.x, p3.y), cv::Point(p4.x, p4.y), red);
			cv::line(m_image.m_data, cv::Point(p4.x, p4.y), cv::Point(p1.x, p1.y), red);
		}

		cv::imshow("TH10", m_image.m_data);
		cv::waitKey(1);
#endif
	}

	// Y��ϵ��
	float_t TH10Bot::GetYFactor(const Pointf& source, const Pointf& next)
	{
		float_t factor;

		if (next.y > source.y)
			factor = 1.0f;
		else if (next.y < source.y)
			factor = -1.0f;
		else
			factor = 0.0f;

		return factor;
	}

	// ����ϵ����Զ��ӷ֣���������
	float_t TH10Bot::GetDistFactor(float_t source, float_t next, float_t target)
	{
		float_t factor;

		float_t dSrc = std::abs(source - target);
		float_t dNext = std::abs(next - target);
		if (dNext > dSrc)
			factor = 1.0f;
		else if (dNext < dSrc)
			factor = -1.0f;
		else
			factor = 0.0f;

		return factor;
	}

	float_t TH10Bot::GetDistXScore(float_t xNext, float_t xTarget)
	{
		float_t dx = std::abs(xNext - xTarget);
		if (dx > SCENE_SIZE.width)
			dx = SCENE_SIZE.width;
		return dx / SCENE_SIZE.width;
	}

	float_t TH10Bot::GetDistYScore(float_t yNext, float_t yTarget)
	{
		float_t dy = std::abs(yNext - yTarget);
		if (dy > SCENE_SIZE.height)
			dy = SCENE_SIZE.height;
		return dy / SCENE_SIZE.height;
	}

	// ����ը��
	bool TH10Bot::handleBomb()
	{
		if (m_input.isKeyPressed('X'))
		{
			m_input.keyRelease('X');
			//std::cout << "ը�� RELEASE" << std::endl;
		}

		// ����ը��3����ټ����ײ
		if (m_clock.getTimestamp() - m_bombCooldown >= 3000)
		{
			if (m_player.status == 4)
			{
				m_input.keyPress('X');
				m_bombCooldown = m_clock.getTimestamp();
				//std::cout << "ը�� PRESS" << std::endl;
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
			// ��ʱ2���Ի�
			if (m_clock.getTimestamp() - m_talkCooldown >= 2000)
			{
				if (m_input.isKeyPressed('Z'))
				{
					m_input.keyRelease('Z');
					//std::cout << "�Ի� RELEASE" << std::endl;
				}
				else
				{
					m_input.keyPress('Z');
					//std::cout << "�Ի� PRESS" << std::endl;
				}
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
			m_input.keyPress('Z');
			m_shootCooldown = m_clock.getTimestamp();
			//std::cout << "���� PRESS" << std::endl;
		}
		else
		{
			// û�е���1���ֹͣ����
			if (m_clock.getTimestamp() - m_shootCooldown >= 1000)
			{
				m_input.keyRelease('Z');
				//std::cout << "���� RELEASE" << std::endl;
				return false;
			}
		}

		return true;
	}

	// �����ƶ�
	bool TH10Bot::handleMove()
	{
		m_itemId = findItem();
		m_enemyId = findEnemy();

		//Node start = {};
		//start.pos = m_player.getPos();
		//start.dir = DIR_NONE;

		//Node goal = {};
		//goal.pos = getMousePos();
		//goal.dir = DIR_NONE;

		//m_path.clear();
		//memset(m_mask, 0, sizeof(m_mask));
		//astar(start, goal);

		if (m_draw)
		{
			m_buffer = cv::Scalar(255, 255, 255);

			cv::Scalar red(0, 0, 255);
			for (const EntityView& view : m_focusBullets)
			{
				const Bullet& bullet = m_bullets[view.index];

				Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
				cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
				cv::rectangle(m_buffer, rect, red, -1);
			}
		}

		Node node;
		node.pos = m_player.getPos();
		node.frame = 0.0f;
		node.target = Scene::FixPos(getMousePos());

		m_count = 0;
		NodeScore score = dfs(node);

		if (score.limit)
			std::cout << "----------------�����ڵ������ơ�" << std::endl;

		//if (score.reach)
		//	std::cout << "����Ŀ�ꡣ" << std::endl;

		if (score.dir != DIR_NONE)
		{
			move(score.dir, score.slow);
		}
		else
		{
			move(DIR_HOLD, false);
			//std::cout << "��·���ߡ�" << std::endl;
		}

		if (m_draw)
		{
			cv::imshow("TH10", m_buffer);
			cv::waitKey(1);
			m_draw = false;
		}

		return true;
	}

	Pointf TH10Bot::getMousePos()
	{
		POINT mousePos = {};
		GetCursorPos(&mousePos);
		Rect clientRect = m_window.getClientRect();
		return Scene::ToScenePos(Pointi(mousePos.x - clientRect.x, mousePos.y - clientRect.y));
	}

	NodeScore TH10Bot::dfs(const Node& node)
	{
		NodeScore score = {};
		score.dir = DIR_NONE;

		score.limit = (++m_count > 2000);
		if (score.limit)
			return score;

		score.inScene = Scene::IsInScene(node.pos);
		if (!score.inScene)
			return score;

		Player player = m_player;
		player.setPos(node.pos);

		score.collide = collideMove(player, node.frame);
		if (score.collide)
			return score;

		score.reach = (player.getDist(node.target) < 10.0f);
		if (score.reach)
			return score;

		//if (m_itemId != -1)
		//	curResult.score += getCollectItemScore(player, m_itemId);
		//else if (m_enemyId != -1)
		//	curResult.score += getShootEnemyScore(player, m_enemyId);
		//else
		//	curResult.score += getGobackScore(player);

		Direction targetDir = player.getDir(node.target);
		Mover mover(targetDir);
		while (mover.hasNext())
		{
			Direction dir = mover.next();
			bool slow = false;

			Node nextNode;
			nextNode.pos = node.pos + MOVE_SPEED[dir];
			nextNode.frame = node.frame + 1.0f;
			nextNode.target = node.target;

			NodeScore nextScore = dfs(nextNode);

			if (nextScore.limit)
			{
				score.limit = true;
				//score.dir = dir;	// ���ߣ�����������
				//score.slow = slow;
				break;
			}

			if (!nextScore.inScene || nextScore.collide)
			{
				slow = true;

				nextNode.pos = node.pos + MOVE_SPEED_SLOW[dir];

				nextScore = dfs(nextNode);

				if (nextScore.limit)
				{
					score.limit = true;
					//score.dir = dir;	// ���ߣ�����������
					//score.slow = slow;
					break;
				}

				if (!nextScore.inScene || nextScore.collide)
					continue;
			}

			if (m_draw)
			{
				Pointi p1 = Scene::ToWindowPos(node.pos);
				Pointi p2 = Scene::ToWindowPos(nextNode.pos);
				cv::line(m_buffer, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), cv::Scalar(0, 255, 0));

				cv::imshow("TH10", m_buffer);
				cv::waitKey(10);
			}

			if (nextScore.reach)
			{
				score.reach = true;
				score.dir = dir;
				score.slow = slow;
				break;
			}
		}

		return score;
	}

	//void TH10Bot::astar(Node& start, Node& goal)
	//{
	//	m_buffer = cv::Scalar(255, 255, 255);

	//	cv::Scalar red(0, 0, 255);
	//	cv::Scalar green(0, 255, 0);
	//	cv::Scalar blue(255, 0, 0);

	//	Pointi windowPos1 = Scene::ToWindowPos(start.pos - Pointf(m_player.width / 2.0f, m_player.height / 2.0f));
	//	cv::Rect rect1(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
	//	cv::rectangle(m_buffer, rect1, green);

	//	windowPos1 = Scene::ToWindowPos(goal.pos - Pointf(m_player.width / 2.0f, m_player.height / 2.0f));
	//	cv::Rect rect2(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
	//	cv::rectangle(m_buffer, rect2, green);

	//	PointNodeMap closedSet;
	//	PointNodeMap openSet;
	//	ScoreNodeMap openScoreSet;

	//	start.gScore = 0.0f;
	//	start.hScore = heuristicCostEstimate(start, goal);
	//	start.fScore = start.gScore + start.hScore;
	//	openSet.insert(std::make_pair(start.pos, start));
	//	openScoreSet.insert(std::make_pair(start.fScore, start));
	//	assert(openSet.size() == openScoreSet.size());

	//	int_t count = 0;
	//	while (!openSet.empty())
	//	{
	//		auto lowestIt = openScoreSet.begin();
	//		//auto maxIt = --(openScoreSet.end());
	//		Node current = lowestIt->second;

	//		// �����յ�
	//		if (current.pos.distance(goal.pos) < 5.0f)
	//		{
	//			reconstructPath(closedSet, current);
	//			break;
	//		}

	//		openSet.erase(current.pos);
	//		openScoreSet.erase(lowestIt);
	//		assert(openSet.size() == openScoreSet.size());
	//		closedSet.insert(std::make_pair(current.pos, current));

	//		++count;
	//		if (count > 1000)
	//			break;

	//		Mover mover(current.fromDir);
	//		while (mover.hasNext())
	//		{
	//			Direction dir = mover.next();

	//			Node neighbor = {};
	//			neighbor.pos = current.pos + MOVE_SPEED[dir];
	//			neighbor.fromPos = current.pos;
	//			neighbor.fromDir = dir;
	//			neighbor.frame = current.frame + 1.0f;

	//			if (!Scene::IsInScene(neighbor.pos))
	//				continue;

	//			if (closedSet.find(neighbor.pos) != closedSet.end())
	//				continue;

	//			Player player = m_player;
	//			player.setPos(neighbor.pos);
	//			if (collideMove(player, neighbor.frame))
	//				continue;

	//			Pointf topLeft = player.getTopLeft();
	//			topLeft.x = std::ceil(topLeft.x);
	//			topLeft.y = std::ceil(topLeft.y);
	//			Pointf topRight = player.getTopRight();
	//			topRight.x = std::floor(topRight.x);
	//			topRight.y = std::ceil(topRight.y);
	//			Pointf bottomLeft = player.getBottomLeft();
	//			bottomLeft.x = std::ceil(bottomLeft.x);
	//			bottomLeft.y = std::floor(bottomLeft.y);
	//			Pointf bottomRight = player.getBottomRight();
	//			bottomRight.x = std::floor(bottomRight.x);
	//			bottomRight.y = std::floor(bottomRight.y);

	//			Pointi t1 = Scene::ToWindowPos(topLeft);
	//			Pointi t2 = Scene::ToWindowPos(topRight);
	//			Pointi t3 = Scene::ToWindowPos(bottomLeft);
	//			Pointi t4 = Scene::ToWindowPos(bottomRight);

	//			bool overflow = false;
	//			for (int_t y = t1.y; y <= t3.y; ++y)
	//				for (int_t x = t1.x; x <= t2.x; ++x)
	//					if (m_mask[y][x] > 2)
	//						overflow = true;
	//			if (overflow)
	//				continue;

	//			for (int_t y = t1.y; y <= t3.y; ++y)
	//				for (int_t x = t1.x; x <= t2.x; ++x)
	//					m_mask[y][x] += 1;

	//			//cv::Rect rect1(t1.x, t1.y, int_t(t2.x - t1.x), int_t(t3.y - t1.y));
	//			//cv::rectangle(m_buffer, rect1, red, -1);

	//			// gScore�ڵ���
	//			neighbor.gScore = current.gScore + distBetween(current, neighbor);
	//			// hScore�ڵݼ�
	//			neighbor.hScore = heuristicCostEstimate(neighbor, goal);
	//			neighbor.fScore = neighbor.gScore + neighbor.hScore;

	//			auto oldIt = openSet.find(neighbor.pos);
	//			if (oldIt == openSet.end())
	//			{
	//				openSet.insert(std::make_pair(neighbor.pos, neighbor));
	//				openScoreSet.insert(std::make_pair(neighbor.fScore, neighbor));
	//				assert(openSet.size() == openScoreSet.size());
	//			}
	//			else
	//			{
	//				Node old = oldIt->second;
	//				if (neighbor.gScore < old.gScore)
	//				{
	//					openSet.erase(oldIt);
	//					openSet.insert(std::make_pair(neighbor.pos, neighbor));
	//					//openScoreSet.erase(old.fScore);
	//					auto range = openScoreSet.equal_range(old.fScore);
	//					auto it = range.first;
	//					while (it != range.second)
	//					{
	//						if (it->second.pos == old.pos)
	//							it = openScoreSet.erase(it);
	//						else
	//							++it;
	//					}
	//					openScoreSet.insert(std::make_pair(neighbor.fScore, neighbor));
	//					assert(openSet.size() == openScoreSet.size());
	//				}
	//			}

	//			//Pointi windowPos1 = Scene::ToWindowPos(neighbor.pos - Pointf(m_player.width / 2.0f, m_player.height / 2.0f));
	//			//cv::Rect rect1(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
	//			//cv::rectangle(m_buffer, rect1, green);

	//			//Pointi p1 = Scene::ToWindowPos(neighbor.pos);
	//			//Pointi p2 = Scene::ToWindowPos(neighbor.fromPos);
	//			//cv::line(m_buffer, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), blue);

	//			//cv::imshow("TH10", m_buffer);
	//			//cv::waitKey(1);
	//		} // while (mover.hasNext())
	//	} // while (!openSet.empty())
	//}

	//float_t TH10Bot::distBetween(const Node& current, const Node& neighbor)
	//{
	//	float_t score = current.pos.distance(neighbor.pos);
	//	return std::round(score * 100.0f) / 100.0f;
	//}

	//float_t TH10Bot::heuristicCostEstimate(const Node& neighbor, const Node& goal)
	//{
	//	float_t score = neighbor.pos.distance(goal.pos);
	//	return std::round(score * 100.0f) / 100.0f;
	//}

	//void TH10Bot::reconstructPath(const PointNodeMap& closedSet, const Node& goal)
	//{
	//	//cv::Scalar green(0, 255, 0);
	//	//cv::Scalar blue(255, 0, 0);

	//	Node current = goal;
	//	while (current.fromDir != DIR_NONE)
	//	{
	//		m_path.push_back(current);

	//		//Pointi windowPos1 = Scene::ToWindowPos(current.pos - Pointf(m_player.width / 2.0f, m_player.height / 2.0f));
	//		//cv::Rect rect1(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
	//		//cv::rectangle(m_buffer, rect1, green);

	//		//Pointi p1 = Scene::ToWindowPos(current.pos);
	//		//Pointi p2 = Scene::ToWindowPos(current.fromPos);
	//		//cv::line(m_buffer, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), blue);

	//		auto fromIt = closedSet.find(current.fromPos);
	//		assert(fromIt != closedSet.end());
	//		current = fromIt->second;
	//	}
	//}

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

	bool TH10Bot::collideMove(const Player& player, float_t frame)
	{
		for (const Enemy& enemy : m_enemies)
		{
			Enemy adv = enemy.advance(frame);
			if (adv.collide(player))
				return true;
		}

		for (const EntityView& view : m_focusBullets)
		{
			const Bullet& bullet = m_bullets[view.index];
			Bullet adv = bullet.advance(frame);
			if (adv.collide(player))
				return true;
		}

		for (const EntityView& view : m_focusLasers)
		{
			const Laser& laser = m_lasers[view.index];
			Laser adv = laser.advance(frame);
			if (adv.collide(player))
				return true;
		}

		return false;
	}

	// ���ҵ���
	int_t TH10Bot::findItem()
	{
		int_t id = -1;

		if (m_player.y < SCENE_SIZE.height / 4.0f)
			return id;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (uint_t i = 0; i < m_items.size(); ++i)
		{
			const Item& item = m_items[i];

			// ����1/5��
			if (item.y < SCENE_SIZE.height / 5.0f)
				continue;

			// �����Ի�������
			float_t dy = std::abs(item.y - m_player.y);
			if (dy > SCENE_SIZE.height / 3.0f)
				continue;

			// ���Ի����������
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

			//  ���Ի�X��������
			float_t dx = std::abs(enemy.x - m_player.x);
			if (dx < minDist)
			{
				minDist = dx;
				id = i;
			}
		}

		return id;
	}

	float_t TH10Bot::getDodgeEnemyScore(const Player& pNext, float_t epsilon)
	{
		float_t score = 0.0f;

		for (const Enemy& enemy : m_enemies)
		{
			if (enemy.collide(pNext))
			{
				score = -10000.0f;
				break;
			}
		}

		return score;
	}

	float_t TH10Bot::getDodgeLaserScore(const Player& pNext, float_t epsilon)
	{
		float_t score = 0.0f;

		for (const Laser& laser : m_lasers)
		{
			if (laser.collide(pNext))
			{
				score = -10000.0f;
				break;
			}
		}

		return score;
	}

	// ʰȡ��������
	float_t TH10Bot::getCollectItemScore(const Player& pNext, int_t itemId)
	{
		float_t score = 0.0f;

		if (itemId == -1)
			return score;

		const Item& item = m_items[itemId];
		if (pNext.getDist(item.getPos()) < 5.0f)
		{
			score += 300.0f;
		}
		else
		{
			score += 150.0f * (1.0f - GetDistXScore(pNext.x, item.x));
			score += 150.0f * (1.0f - GetDistYScore(pNext.y, item.y));
		}

		return score;
	}

	// ������������
	float_t TH10Bot::getShootEnemyScore(const Player& pNext, int_t enemyId)
	{
		float_t score = 0.0f;

		if (enemyId == -1)
			return score;

		const Enemy& enemy = m_enemies[enemyId];
		float_t dx = std::abs(pNext.x - enemy.x);
		if (dx < 15.0f)
		{
			score += 300.0f;
		}
		else
		{
			// X�����ԽԶ�÷�Խ��
			if (dx > SCENE_SIZE.width)
				dx = SCENE_SIZE.width;
			score += 300.0f * (1.0f - dx / SCENE_SIZE.width);
		}

		return score;
	}

	float_t TH10Bot::getGobackScore(const Player& pNext)
	{
		float_t score = 0.0f;

		if (pNext.getPos().distance(PLAYER_INIT_POS) < 10.0f)
		{
			score += 30.0f;
		}
		else
		{
			score += 15.0f * (1.0f - GetDistXScore(pNext.x, PLAYER_INIT_POS.x));
			score += 15.0f * (1.0f - GetDistYScore(pNext.y, PLAYER_INIT_POS.y));
		}

		return score;
	}

	void TH10Bot::move(Direction dir, bool slow)
	{
		if (slow)
		{
			m_input.keyPress(VK_SHIFT);
			//std::cout << "���� PRESS" << std::endl;
		}
		else
		{
			m_input.keyRelease(VK_SHIFT);
			//std::cout << "���� RELEASE" << std::endl;
		}

		switch (dir)
		{
		case DIR_HOLD:
			m_input.keyRelease(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			//std::cout << "����" << std::endl;
			break;

		case DIR_UP:
			m_input.keyPress(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			//std::cout << "��" << std::endl;
			break;

		case DIR_DOWN:
			m_input.keyRelease(VK_UP);
			m_input.keyPress(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			//std::cout << "��" << std::endl;
			break;

		case DIR_LEFT:
			m_input.keyRelease(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyPress(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			//std::cout << "��" << std::endl;
			break;

		case DIR_RIGHT:
			m_input.keyRelease(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyPress(VK_RIGHT);
			//std::cout << "��" << std::endl;
			break;

		case DIR_UPLEFT:
			m_input.keyPress(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyPress(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			//std::cout << "����" << std::endl;
			break;

		case DIR_UPRIGHT:
			m_input.keyPress(VK_UP);
			m_input.keyRelease(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyPress(VK_RIGHT);
			//std::cout << "����" << std::endl;
			break;

		case DIR_DOWNLEFT:
			m_input.keyRelease(VK_UP);
			m_input.keyPress(VK_DOWN);
			m_input.keyPress(VK_LEFT);
			m_input.keyRelease(VK_RIGHT);
			//std::cout << "����" << std::endl;
			break;

		case DIR_DOWNRIGHT:
			m_input.keyRelease(VK_UP);
			m_input.keyPress(VK_DOWN);
			m_input.keyRelease(VK_LEFT);
			m_input.keyPress(VK_RIGHT);
			//std::cout << "����" << std::endl;
			break;
		}
	}

	// �������
	//bool TH10Bot::handleItem()
	//{
	//	if (checkCollectStatus())
	//	{
	//		int_t itemId = findItem();
	//		if (itemId != -1)
	//			return collectItem(itemId);
	//	}
	//	return false;
	//}

	// ���ʰȡ״��
	//bool TH10Bot::checkCollectStatus()
	//{
	//	// ʰȡ��ȴ��
	//	if (m_clock.getTimestamp() - m_collectCooldown < 2000)
	//		return false;
	//
	//	// �޵���
	//	if (m_items.empty())
	//	{
	//		// ������ȴ
	//		m_collectCooldown = m_clock.getTimestamp();
	//		return false;
	//	}
	//
	//	// �Ի����ϰ�������������10�������˶���2��
	//	if (m_player.y < SCENE_HEIGHT / 2.0 && m_items.size() < 10 && m_enemies.size() > 2)
	//	{
	//		// ������ȴ
	//		m_collectCooldown = m_clock.getTimestamp();
	//		return false;
	//	}
	//
	//	// �Ի�����1/4��
	//	if (m_player.y < SCENE_HEIGHT * 0.25)
	//	{
	//		// ������ȴ
	//		m_collectCooldown = m_clock.getTimestamp();
	//		return false;
	//	}
	//
	//	return true;
	//}

	// ʰȡ����
	//bool TH10Bot::collectItem(int_t itemId)
	//{
	//	const Item& item = m_items[itemId];
	//
	//	// ����������
	//	if (m_player.collide(item, 5.0))
	//		return true;
	//
	//	int_t lastDir = DIR_NONE;
	//	bool lastShift = false;
	//	float_t maxScore = std::numeric_limits<float_t>::lowest();
	//	for (int_t i = 0; i < DIR_LENGHT; ++i)
	//	{
	//		int_t dir = DIRECTIONS[i];
	//		bool shift = false;
	//		float_t score = 0.0;
	//
	//		float_t xNext = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED;
	//		float_t yNext = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED;
	//		FixPos(xNext, yNext);
	//		Player next = { static_cast<float_t>(xNext), static_cast<float_t>(yNext), m_player.w, m_player.h };
	//		if (collide(next, 0.0))
	//		{
	//			shift = true;
	//			xNext = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED_SLOW;
	//			yNext = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED_SLOW;
	//			FixPos(xNext, yNext);
	//			next = { static_cast<float_t>(xNext), static_cast<float_t>(yNext), m_player.w, m_player.h };
	//			if (collide(next, 0.0))
	//				continue;
	//		}
	//
	//		score += collectItemScore(next, item);
	//
	//		if (score > maxScore)
	//		{
	//			maxScore = score;
	//			lastDir = dir;
	//			lastShift = shift;
	//		}
	//	}
	//	if (lastDir != DIR_NONE)
	//		move(lastDir, lastShift);
	//	else
	//		std::cout << "collectItem()��·����" << std::endl;
	//
	//	return true;
	//}

	// �������
	//bool TH10Bot::handleEnemy()
	//{
	//	if (checkShootStatus())
	//	{
	//		int_t enemyId = findEnemy();
	//		if (enemyId != -1)
	//			return shootEnemy(enemyId);
	//	}
	//	return false;
	//}

	// ��⹥��״��
	//bool TH10Bot::checkShootStatus()
	//{
	//	// �޵���
	//	if (m_enemies.empty())
	//	{
	//		return false;
	//	}
	//
	//	return true;
	//}

	// ��������
	//bool TH10Bot::shootEnemy(int_t enemyId)
	//{
	//	const Enemy& enemy = m_enemies[enemyId];
	//
	//	int_t lastDir = DIR_NONE;
	//	bool lastShift = false;
	//	float_t maxScore = 1e-15;
	//	for (int_t i = 0; i < DIR_LENGHT; ++i)
	//	{
	//		int_t dir = DIRECTIONS[i];
	//		bool shift = false;
	//		float_t score = 0.0;
	//
	//		float_t xNext = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED;
	//		float_t yNext = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED;
	//		FixPos(xNext, yNext);
	//		Player next = { static_cast<float_t>(xNext), static_cast<float_t>(yNext), m_player.w, m_player.h };
	//		if (collide(next, 0.0))
	//		{
	//			shift = true;
	//			xNext = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED_SLOW;
	//			yNext = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED_SLOW;
	//			FixPos(xNext, yNext);
	//			next = { static_cast<float_t>(xNext), static_cast<float_t>(yNext), m_player.w, m_player.h };
	//			if (collide(next, 0.0))
	//				continue;
	//		}
	//
	//		score += dodgeEnemyScore(next);
	//		score += shootEnemyScore(next, enemy);
	//
	//		if (score > maxScore)
	//		{
	//			maxScore = score;
	//			lastDir = dir;
	//			lastShift = shift;
	//		}
	//	}
	//	if (lastDir != DIR_NONE)
	//		move(lastDir, lastShift);
	//	else
	//		std::cout << "shootEnemy()��·����" << std::endl;
	//
	//	return true;
	//}

	// ���ܵ�������
	//float_t TH10Bot::dodgeEnemyScore(const Player& next)
	//{
	//	float_t allScore = 0.0;
	//	for (const Enemy& enemy : m_enemies)
	//	{
	//		float_t score = 0.0;
	//
	//		// �ڵл���Χ50�� 
	//		if (!next.collide(enemy, 50.0))
	//		{
	//			score += 100.0;
	//		}
	//		else
	//		{
	//			Point2d dirFactor = GetDirFactor(m_player, next, enemy);
	//			score += (dirFactor.x * 50.0 + dirFactor.y * 50.0);
	//		}
	//
	//		score += GetYFactor(m_player, next) * 100.0;
	//
	//		allScore += score;
	//	}
	//	return allScore;
	//}

	// ��λ
	//void TH10Bot::goback()
	//{
	//	// ������ʼλ����
	//	if (m_player.collide(INIT_RECT, 5.0))
	//	{
	//		move(DIR_CENTER, false);
	//		return;
	//	}
	//
	//	int_t lastDir = DIR_NONE;
	//	bool lastShift = false;
	//	float_t maxScore = 1e-15;
	//	for (int_t i = 0; i < DIR_LENGHT; ++i)
	//	{
	//		int_t dir = DIRECTIONS[i];
	//		bool shift = false;
	//		float_t score = 0.0;
	//
	//		float_t nextX = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED;
	//		float_t nextY = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED;
	//		FixPos(nextX, nextY);
	//		Player player = { static_cast<float_t>(nextX), static_cast<float_t>(nextY), m_player.w, m_player.h };
	//		if (collide(player, 0.0))
	//		{
	//			shift = true;
	//			nextX = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED_SLOW;
	//			nextY = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED_SLOW;
	//			FixPos(nextX, nextY);
	//			player = { static_cast<float_t>(nextX), static_cast<float_t>(nextY), m_player.w, m_player.h };
	//			if (collide(player, 0.0))
	//				continue;
	//		}
	//
	//		score += 1.0 - GetDistScore(player, INIT_RECT);
	//
	//		//std::cout << (int_t)dir << " " << score << std::endl;
	//		if (score > maxScore)
	//		{
	//			maxScore = score;
	//			lastDir = dir;
	//			lastShift = shift;
	//		}
	//	}
	//	//std::cout << "last " << (int_t)lastDir << " " << lastShift << std::endl;
	//	if (lastDir != DIR_NONE)
	//		move(lastDir, lastShift);
	//	else
	//		std::cout << "goback()��·����" << std::endl;
	//}
}

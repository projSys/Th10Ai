#include "TH10Bot/Common.h"
#include "TH10Bot/TH10Bot.h"

#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

namespace th
{
	TH10Bot::TH10Bot() :
		m_process(Process::FindByName("th10.exe")),
		m_window(Window::FindByClassName("BASE")),
		m_sync(m_process),
		m_reader(m_process),
		m_active(false),
		m_keyUp(VK_UP), m_keyDown(VK_DOWN), m_keyLeft(VK_LEFT), m_keyRight(VK_RIGHT),
		m_keyShift(VK_SHIFT), m_keyZ('Z'), m_keyX('X'),
		//m_depthList(DEPTH),
		//m_clipList(DEPTH),
		m_bombCooldown(0),
		m_talkCooldown(0),
		m_shootCooldown(0),
		m_pickupCooldown(0)
	{
		srand((unsigned int)time(nullptr));
	}

	TH10Bot::~TH10Bot()
	{
	}

	void TH10Bot::start()
	{
		if (!m_active)
		{
			m_active = true;
			std::cout << "开启Bot。" << std::endl;
		}
	}

	void TH10Bot::stop()
	{
		if (m_active)
		{
			m_active = false;
			m_keyUp.release(), m_keyDown.release(), m_keyLeft.release(), m_keyRight.release(),
				m_keyShift.release(), m_keyZ.release(), m_keyX.release();
			std::cout << "停止Bot。" << std::endl;
		}
	}

	void TH10Bot::update()
	{
		if (!m_active)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			return;
		}

		if (!m_sync.waitForPresent())
		{
			std::cout << "等待帧同步超时。" << std::endl;
			return;
		}

		//Rect rect = m_window.getClientRect();
		//if (!m_capturer.capture(m_image, rect))
		//{
		//	std::cout << "抓图失败：桌面没有变化导致超时，或者窗口位置超出桌面范围。" << std::endl;
		//	return;
		//}

		//std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();

		m_clock.update();

		m_reader.getPlayer(m_player);
		m_reader.getItems(m_items);
		m_reader.getEnemies(m_enemies);
		m_reader.getBullets(m_bullets);
		m_reader.getLasers(m_lasers);

		// 裁剪弹幕
		m_redList.clear();
		m_yellowList.clear();
		for (uint_t i = 0; i < m_bullets.size(); ++i)
		{
			const Bullet& bullet = m_bullets[i];
			float_t distance = bullet.distance(m_player);
			if (distance < CLIP_DISTANCE)
			{
				float_t footFrame = bullet.footFrame(m_player.getPos());
				Pointf footPoint = bullet.footPoint(footFrame);
				float_t angleOfPlayer = bullet.angle(m_player);
				m_redList.emplace_back(i, distance, footFrame, footPoint, angleOfPlayer);
			}
			else
			{
				float_t footFrame = bullet.footFrame(m_player.getPos());
				Pointf footPoint = bullet.footPoint(footFrame);
				if (m_player.distance(footPoint) < CLIP_DISTANCE)
				{
					float_t angleOfPlayer = bullet.angle(m_player);
					if (angleOfPlayer < CLIP_ANGLE90)
						m_yellowList.emplace_back(i, distance, footFrame, footPoint, angleOfPlayer);
				}
			}
		}

		std::sort(m_redList.begin(), m_redList.end(),
			[](const BulletLv1& left, const BulletLv1& right)->bool
		{
			return left.distance < right.distance;
		});
		std::sort(m_yellowList.begin(), m_yellowList.end(),
			[](const BulletLv1& left, const BulletLv1& right)->bool
		{
			return left.footFrame < right.footFrame;
		});

		for (const BulletLv1& it : m_redList)
		{
			Bullet& bullet = m_bullets[it.index];

			Pointf oldPos = bullet.getPos();
			bullet.setPos(it.footPoint);
			// 自机与子弹前进路径的垂足距离最近，只要与垂足不碰撞，那么与路径上的其他点都不会碰撞。
			if (!bullet.hitTest(m_player))
			{
				bullet.setPos(oldPos);
				continue;
			}
			bullet.setPos(oldPos);

			int_t minFrame = -1;
			oldPos = bullet.getPos();
			for (int_t i = static_cast<int_t>(it.footFrame); i > 0; --i)
			{
				Pointf framePoint = bullet.advanceFrame(i);
				bullet.setPos(framePoint);
				if (bullet.hitTest(m_player))
					minFrame = i;
				else
					break;
			}
			bullet.setPos(oldPos);

			Pointf xPos = bullet.getPos();
			xPos.x += SCENE_SIZE.width;	// 按X轴平移
			float_t angleOfXAxis = bullet.angle(xPos);
			if (bullet.dy > 0) // 转换成360度
				angleOfXAxis = 360.0f - angleOfXAxis;

			Direction dir = DIR_NONE;
			if (angleOfXAxis > 337.5f)
				dir = DIR_RIGHT;
			else if (angleOfXAxis > 292.5f)
				dir = DIR_DOWNRIGHT;
			else if (angleOfXAxis > 247.5f)
				dir = DIR_DOWN;
			else if (angleOfXAxis > 202.5f)
				dir = DIR_DOWNLEFT;
			else if (angleOfXAxis > 157.5f)
				dir = DIR_LEFT;
			else if (angleOfXAxis > 112.5f)
				dir = DIR_UPLEFT;
			else if (angleOfXAxis > 67.5f)
				dir = DIR_UP;
			else if (angleOfXAxis > 22.5f)
				dir = DIR_UPRIGHT;
			else
				dir = DIR_RIGHT;

			switch (dir)
			{
			case DIR_UP:
				move(rand() % 2 == 0 ? DIR_LEFT : DIR_RIGHT);
				break;
			case DIR_DOWN:
				move(rand() % 2 == 0 ? DIR_LEFT : DIR_RIGHT);
				break;
			case DIR_LEFT:
				move(rand() % 2 == 0 ? DIR_UP : DIR_DOWN);
				break;
			case DIR_RIGHT:
				move(rand() % 2 == 0 ? DIR_UP : DIR_DOWN);
				break;
			case DIR_UPLEFT:
				move(rand() % 2 == 0 ? DIR_UPRIGHT : DIR_DOWNLEFT);
				break;
			case DIR_UPRIGHT:
				move(rand() % 2 == 0 ? DIR_UPLEFT : DIR_DOWNRIGHT);
				break;
			case DIR_DOWNLEFT:
				move(rand() % 2 == 0 ? DIR_UPLEFT : DIR_DOWNRIGHT);
				break;
			case DIR_DOWNRIGHT:
				move(rand() % 2 == 0 ? DIR_UPRIGHT : DIR_DOWNLEFT);
				break;
			}

			break;
		}

		return;

		cv::Scalar red(0, 0, 255);
		cv::Scalar green(0, 255, 0);
		cv::Scalar blue(255, 0, 0);
		cv::Scalar orange(0, 97, 255);
		cv::Scalar yellow(0, 255, 255);

		Pointi windowPos1 = Scene::ToWindowPos(m_player.getTopLeft());
		cv::Rect rect1(windowPos1.x, windowPos1.y, int_t(m_player.width), int_t(m_player.height));
		cv::rectangle(m_image.m_data, rect1, green, -1);
		Pointi windowPos11 = Scene::ToWindowPos(m_player.getPos());
		cv::Point center11(windowPos11.x, windowPos11.y);
		cv::circle(m_image.m_data, center11, int_t(CLIP_DISTANCE), green);

		//for (const Item& item : m_items)
		//{
		//	Pointi windowPos = Scene::ToWindowPos(item.getLeftTop());
		//	cv::Rect rect(windowPos.x, windowPos.y, int_t(item.width), int_t(item.height));
		//	cv::rectangle(m_image.m_data, rect, blue, -1);
		//}

		for (const Enemy& enemy : m_enemies)
		{
			Pointi windowPos = Scene::ToWindowPos(enemy.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(enemy.width), int_t(enemy.height));
			cv::rectangle(m_image.m_data, rect, red);
		}
		for (const BulletLv1& it : m_redList)
		{
			const Bullet& bullet = m_bullets[it.index];

			Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
			cv::rectangle(m_image.m_data, rect, red, -1);
		}
		for (const BulletLv1& it : m_yellowList)
		{
			const Bullet& bullet = m_bullets[it.index];

			Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
			cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
			cv::rectangle(m_image.m_data, rect, yellow, -1);

			Pointi p1 = Scene::ToWindowPos(m_player.getPos());
			Pointi p2 = Scene::ToWindowPos(bullet.getPos());
			Pointi p3 = Scene::ToWindowPos(it.footPoint);
			//cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), yellow);
			cv::line(m_image.m_data, cv::Point(p1.x, p1.y), cv::Point(p3.x, p3.y), yellow);
			cv::line(m_image.m_data, cv::Point(p2.x, p2.y), cv::Point(p3.x, p3.y), yellow);
		}

		cv::imshow("TH10", m_image.m_data);
		cv::waitKey(1);

		return;

		//std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		//time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
		//std::cout << "e1: " << e1 << std::endl;

		handleBomb();
		if (!handleTalk())
			handleShoot();
		handleMove();

		//std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		//time_t e2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		//std::cout << "e2: " << e2 << std::endl;
	}

	// Y轴系数
	float_t TH10Bot::GetYFactor(const Pointf& source, const Pointf& next)
	{
		float_t factor;

		if (next.y > source.y)
			factor = 1.0;
		else if (next.y < source.y)
			factor = -1.0;
		else
			factor = 0.0;

		return factor;
	}

	// 距离系数，远离加分，靠近减分
	float_t TH10Bot::GetDistFactor(float_t source, float_t next, float_t target)
	{
		float_t factor;

		float_t dSrc = std::abs(source - target);
		float_t dNext = std::abs(next - target);
		if (dNext > dSrc)
			factor = 1.0;
		else if (dNext < dSrc)
			factor = -1.0;
		else
			factor = 0.0;

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

	// 处理炸弹
	bool TH10Bot::handleBomb()
	{
		if (m_keyX.isPressed())
		{
			m_keyX.release();
			//std::cout << "炸弹 RELEASE" << std::endl;
		}

		// 放了炸弹3秒后再检测碰撞
		if (m_clock.getTimestamp() - m_bombCooldown >= 3000)
		{
			if (hitTestBomb())
			{
				m_bombCooldown = m_clock.getTimestamp();
				m_keyX.press();
				//std::cout << "炸弹 PRESS" << std::endl;
				//std::cout << m_log.str() << std::endl;
				return true;
			}
		}

		return false;
	}

	bool TH10Bot::hitTestBomb()
	{
		for (const Enemy& enemy : m_enemies)
		{
			if (m_player.hitTest(enemy))
				return true;
		}

		for (const Bullet& bullet : m_bullets)
		{
			if (m_player.hitTest(bullet))
				return true;
		}

		for (const Laser& laser : m_lasers)
		{
			if (m_player.hitTestSAT(laser))
				return true;
		}

		return false;
	}

	// 处理对话
	bool TH10Bot::handleTalk()
	{
		if (m_enemies.size() > 1 || !m_bullets.empty() || !m_lasers.empty())
		{
			m_talkCooldown = m_clock.getTimestamp();
		}
		else
		{
			// 延时2秒后对话
			if (m_clock.getTimestamp() - m_talkCooldown >= 2000)
			{
				if (m_keyZ.isPressed())
				{
					m_keyZ.release();
					//std::cout << "对话 RELEASE" << std::endl;
				}
				else
				{
					m_keyZ.press();
					//std::cout << "对话 PRESS" << std::endl;
				}
				return true;
			}
		}

		return false;
	}

	// 处理攻击
	bool TH10Bot::handleShoot()
	{
		if (!m_enemies.empty())
		{
			m_shootCooldown = m_clock.getTimestamp();
			m_keyZ.press();
			//std::cout << "攻击 PRESS" << std::endl;
		}
		else
		{
			// 没有敌人1秒后停止攻击
			if (m_clock.getTimestamp() - m_shootCooldown >= 1000)
			{
				m_keyZ.release();
				//std::cout << "攻击 RELEASE" << std::endl;
				return false;
			}
		}

		return true;
	}

	// 处理移动
	bool TH10Bot::handleMove()
	{
		//m_depthList[0].clear();
		//m_depthList[0].assign(m_bullets.begin(), m_bullets.end());
		//for (int_t i = 1; i < DEPTH; ++i)
		//{
		//	m_depthList[i].clear();
		//	m_clipList[i].clear();
		//	Rect2d moveAera(m_player.x, m_player.y, m_player.w + MOVE_SPEED[0] * 2 * i, m_player.h + MOVE_SPEED[0] * 2 * i);
		//	for (const Bullet& bullet : m_depthList[i - 1])
		//	{
		//		Bullet next = bullet.advance();
		//		m_depthList[i].push_back(next);
		//		if (moveAera.hitTest(next, 0.0))
		//			m_clipList[i].push_back(next);
		//	}
		//}

		//int_t powerId = findPower();
		//int_t enemyId = findEnemy();
		Pointf mousePos = getMousePos();

		Direction lastDir = DIR_NONE;
		bool lastSlow = false;
		float_t maxScore = std::numeric_limits<float_t>::lowest();
		//int_t depth = 0;

		for (int_t i = DIR_HOLD; i < DIR_MAXCOUNT; ++i)
		{
			Direction dir = DIRECTIONS[i];
			assert(dir == i);

			float_t nextX, nextY;
			if (!IsSlow(dir))
			{
				nextX = m_player.x + DIR_FACTORS[dir].x * MOVE_SPEED;
				nextY = m_player.y + DIR_FACTORS[dir].y * MOVE_SPEED;
			}
			else
			{
				nextX = m_player.x + DIR_FACTORS[dir].x * MOVE_SPEED_SLOW;
				nextY = m_player.y + DIR_FACTORS[dir].y * MOVE_SPEED_SLOW;
			}
			if (!Scene::IsInScene(Pointf(nextX, nextY)))
				continue;

			Player nextPlayer(nextX, nextY, m_player.width, m_player.height, m_player.dx, m_player.dy);
			if (hitTestMove(nextPlayer))
				continue;

			float_t score = 0.0;
			score += getTargetScore(nextPlayer, mousePos);

			//score += search(pNext, depth + 1);

			//score += getDodgeEnemyScore(pNext);
			//score += getDodgeBulletScore(pNext);
			//score += getDodgeLaserScore(pNext);
			//if (powerId != -1)
			//	score += getPickupPowerScore(pNext, powerId);
			//else if (enemyId != -1)
			//	score += getShootEnemyScore(pNext, enemyId);
			//else
			//	score += getGobackScore(pNext);

			if (score > maxScore)
			{
				maxScore = score;
				lastDir = dir;
			}
		}

		move(lastDir);
		return true;
	}

	Pointf TH10Bot::getMousePos()
	{
		POINT mousePos = {};
		GetCursorPos(&mousePos);
		Rect clientRect = m_window.getClientRect();
		return Scene::ToGamePos(Pointi(mousePos.x - clientRect.x, mousePos.y - clientRect.y));
	}

	bool TH10Bot::hitTestMove(const Player& player)
	{
		for (const Enemy& enemy : m_enemies)
		{
			if (player.hitTest(enemy, 2.0))
				return true;
		}

		for (const Bullet& bullet : m_bullets)
		{
			if (player.hitTest(bullet, 2.0))
				return true;

			Bullet next = bullet.advance();
			if (player.hitTest(next, 2.0))
				return true;
		}

		for (const Laser& laser : m_lasers)
		{
			if (player.hitTestSAT(laser, 2.0))
				return true;
		}

		return false;
	}

	float_t TH10Bot::getTargetScore(const Player& pNext, const Pointf& target)
	{
		float_t score = 0.0;

		if (pNext.distance(target) < 10.0)
		{
			score += 3000.0;
		}
		else
		{
			score += 1500.0 * (1.0 - GetDistXScore(pNext.x, target.x));
			score += 1500.0 * (1.0 - GetDistYScore(pNext.y, target.y));
		}

		return score;
	}
	/*
	float_t TH10Bot::search(const Player& player, int_t depth)
	{
		float_t score = 0.0;

		//Rect2d moveAera(player.x, player.y, player.w + MOVE_SPEED[0] * 2.0, player.h + MOVE_SPEED[0] * 2.0);
		//m_clipList[depth].clear();
		//for (const Bullet& bullet : m_depthList[depth])
		//{
		//	if (moveAera.hitTest(bullet, 0.0))
		//		m_clipList[depth].push_back(bullet);
		//}

		for (int_t m = 0; m < MOVE_SPEED_LEN - 1; ++m)
		{
			for (int_t d = 1; d < DIR_LEN; ++d)
			{
				int_t dir = DIRECTION[d];
				bool slow = static_cast<bool>(m);

				// 下一个位置
				float_t xNext = player.x + DIR_FACTOR[d].x * MOVE_SPEED[m];
				float_t yNext = player.y + DIR_FACTOR[d].y * MOVE_SPEED[m];

				//FixPos(xNext, yNext);
				if (xNext < -200.0 || xNext > 200.0 || yNext < 0.0 || yNext > 480.0)
					continue;

				Player pNext(xNext, yNext, m_player.width, m_player.height, m_player.dx, m_player.dy);

				// 剪枝：不折返
				if (m_player.distance(pNext) < m_player.distance(player))
					continue;

				if (hitTestMove(pNext, depth))
					continue;

				score += 1.0;

				//if (powerId != -1)
				//	score += getPickupPowerScore(pNext, powerId);
				//else if (enemyId != -1)
				//	score += getShootEnemyScore(pNext, enemyId);
				//else
				//	score += getGobackScore(pNext);

				if (depth + 1 < DEPTH)
					score += search(pNext, depth + 1);
			}
		}

		return score;
	}

	bool TH10Bot::hitTestMove(const Player& player, int_t depth)
	{
		if (depth == 0)
		{
			if (player.hitTest(m_enemies, 2.0))
				return true;

			if (player.hitTest(m_bullets, 2.0))
				return true;

			if (player.hitTestSAT(m_lasers, 2.0))
				return true;
		}
		else
		{
			// Enemy缺失

			if (player.hitTest(m_clipList[depth], 2.0))
				return true;

			// Laser缺失
		}

		return false;
	}
	*/
	// 查找道具
	int_t TH10Bot::findPower()
	{
		int_t id = -1;

		if (m_player.y < SCENE_SIZE.height / 4)
			return id;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (uint_t i = 0; i < m_items.size(); ++i)
		{
			const Item& item = m_items[i];

			// 高于1/5屏
			if (item.y < SCENE_SIZE.height / 5)
				continue;

			// 不在自机半屏内
			float_t dy = std::abs(m_player.y - item.y);
			if (dy > SCENE_SIZE.height / 4)
				continue;

			// 与自机距离最近的
			float_t distance = m_player.distance(item);
			if (distance < minDist)
			{
				minDist = distance;
				id = i;
			}
		}

		return id;
	}

	// 查找敌人
	int_t TH10Bot::findEnemy()
	{
		int_t id = -1;

		if (m_player.y < SCENE_SIZE.height / 4)
			return id;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (uint_t i = 0; i < m_enemies.size(); ++i)
		{
			const Enemy& enemy = m_enemies[i];

			//  与自机X轴距离最近
			float_t dx = std::abs(m_player.x - enemy.x);
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
		float_t score = 0.0;

		for (const Enemy& enemy : m_enemies)
		{
			if (pNext.hitTest(enemy, epsilon))
			{
				score = -10000.0;
				break;
			}
		}

		return score;
	}

	// 躲闪子弹评分
	// 帧同步后还是没法准确地碰撞检测，只好不要靠子弹那么近
	float_t TH10Bot::getDodgeBulletScore(const Player& pNext, float_t epsilon)
	{
		float_t score = 0.0;

		for (const Bullet& bullet : m_bullets)
		{
			if (pNext.hitTest(bullet, epsilon))
			{
				score = -10000.0;
				break;
			}

			Bullet bNext = bullet.advance();
			if (pNext.hitTest(bNext, epsilon))
			{
				score = -10000.0;
				break;
			}
		}

		return score;
	}

	float_t TH10Bot::getDodgeLaserScore(const Player& pNext, float_t epsilon)
	{
		float_t score = 0.0;

		for (const Laser& laser : m_lasers)
		{
			if (pNext.hitTestSAT(laser, epsilon))
			{
				score = -10000.0;
				break;
			}
		}

		return score;
	}

	float_t TH10Bot::getBulletAngleScore(const Player& pNext)
	{
		float_t score = 0.0;

		for (const Bullet& bullet : m_bullets)
		{
			float_t angle = bullet.angle(pNext);
			score = 10000 * (1.0 - angle / 180.0);
		}

		return score;
	}

	// 拾取道具评分
	float_t TH10Bot::getPickupPowerScore(const Player& pNext, int_t powerId)
	{
		float_t score = 0.0;

		if (powerId == -1)
			return score;

		const Item& item = m_items[powerId];
		if (pNext.hitTest(item, 5.0))
		{
			score += 3000.0;
		}
		else
		{
			score += 1500.0 * (1.0 - GetDistXScore(pNext.x, item.x));
			score += 1500.0 * (1.0 - GetDistYScore(pNext.y, item.y));
		}

		return score;
	}

	// 攻击敌人评分
	float_t TH10Bot::getShootEnemyScore(const Player& pNext, int_t enemyId)
	{
		float_t score = 0.0;

		if (enemyId == -1)
			return score;

		const Enemy& enemy = m_enemies[enemyId];
		float_t dx = std::abs(pNext.x - enemy.x);
		if (dx < 15.0)
		{
			score += 300.0;
		}
		else
		{
			// X轴距离越远得分越少
			if (dx > SCENE_SIZE.width)
				dx = SCENE_SIZE.width;
			score += 300.0 * (1.0 - dx / SCENE_SIZE.width);
		}

		return score;
	}

	float_t TH10Bot::getGobackScore(const Player& pNext)
	{
		float_t score = 0.0;

		if (pNext.distance(INIT_POS) < 10.0)
		{
			score += 30.0;
		}
		else
		{
			score += 15.0 * (1.0 - GetDistXScore(pNext.x, INIT_POS.x));
			score += 15.0 * (1.0 - GetDistYScore(pNext.y, INIT_POS.y));
		}

		return score;
	}

	bool TH10Bot::IsSlow(Direction dir)
	{
		return (dir == DIR_HOLD_SLOW) || (dir >= DIR_UP_SLOW && dir <= DIR_DOWNRIGHT_SLOW);
	}

	void TH10Bot::move(Direction dir)
	{
		if (IsSlow(dir))
		{
			m_keyShift.press();
			//std::cout << "慢速 PRESS" << std::endl;
		}
		else
		{
			m_keyShift.release();
			//std::cout << "慢速 RELEASE" << std::endl;
		}

		switch (dir)
		{
		case DIR_HOLD_SLOW:
		case DIR_HOLD:
			m_keyUp.release(), m_keyDown.release(), m_keyLeft.release(), m_keyRight.release();
			//std::cout << "不动" << std::endl;
			break;

		case DIR_UP:
		case DIR_UP_SLOW:
			m_keyUp.press(), m_keyDown.release(), m_keyLeft.release(), m_keyRight.release();
			//std::cout << "上" << std::endl;
			break;

		case DIR_DOWN:
		case DIR_DOWN_SLOW:
			m_keyUp.release(), m_keyDown.press(), m_keyLeft.release(), m_keyRight.release();
			//std::cout << "下" << std::endl;
			break;

		case DIR_LEFT:
		case DIR_LEFT_SLOW:
			m_keyUp.release(), m_keyDown.release(), m_keyLeft.press(), m_keyRight.release();
			//std::cout << "左" << std::endl;
			break;

		case DIR_RIGHT:
		case DIR_RIGHT_SLOW:
			m_keyUp.release(), m_keyDown.release(), m_keyLeft.release(), m_keyRight.press();
			//std::cout << "右" << std::endl;
			break;

		case DIR_UPLEFT:
		case DIR_UPLEFT_SLOW:
			m_keyUp.press(), m_keyDown.release(), m_keyLeft.press(), m_keyRight.release();
			//std::cout << "左上" << std::endl;
			break;

		case DIR_UPRIGHT:
		case DIR_UPRIGHT_SLOW:
			m_keyUp.press(), m_keyDown.release(), m_keyLeft.release(), m_keyRight.press();
			//std::cout << "右上" << std::endl;
			break;

		case DIR_DOWNLEFT:
		case DIR_DOWNLEFT_SLOW:
			m_keyUp.release(), m_keyDown.press(), m_keyLeft.press(), m_keyRight.release();
			//std::cout << "左下" << std::endl;
			break;

		case DIR_DOWNRIGHT:
		case DIR_DOWNRIGHT_SLOW:
			m_keyUp.release(), m_keyDown.press(), m_keyLeft.release(), m_keyRight.press();
			//std::cout << "右下" << std::endl;
			break;
		}
	}

	// 处理道具
	//bool TH10Bot::handlePower()
	//{
	//	if (checkPickupStatus())
	//	{
	//		int_t powerId = findPower();
	//		if (powerId != -1)
	//			return pickupPower(powerId);
	//	}
	//	return false;
	//}

	// 检测拾取状况
	//bool TH10Bot::checkPickupStatus()
	//{
	//	// 拾取冷却中
	//	if (m_clock.getTimestamp() - m_pickupCooldown < 2000)
	//		return false;
	//
	//	// 无道具
	//	if (m_powers.empty())
	//	{
	//		// 进入冷却
	//		m_pickupCooldown = m_clock.getTimestamp();
	//		return false;
	//	}
	//
	//	// 自机在上半屏，道具少于10个，敌人多于2个
	//	if (m_player.y < SCENE_HEIGHT / 2.0 && m_powers.size() < 10 && m_enemies.size() > 2)
	//	{
	//		// 进入冷却
	//		m_pickupCooldown = m_clock.getTimestamp();
	//		return false;
	//	}
	//
	//	// 自机高于1/4屏
	//	if (m_player.y < SCENE_HEIGHT * 0.25)
	//	{
	//		// 进入冷却
	//		m_pickupCooldown = m_clock.getTimestamp();
	//		return false;
	//	}
	//
	//	return true;
	//}

	// 拾取道具
	//bool TH10Bot::pickupPower(int_t powerId)
	//{
	//	const Power& power = m_powers[powerId];
	//
	//	// 靠近道具了
	//	if (m_player.hitTest(power, 5.0))
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
	//		if (hitTest(next, 0.0))
	//		{
	//			shift = true;
	//			xNext = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED_SLOW;
	//			yNext = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED_SLOW;
	//			FixPos(xNext, yNext);
	//			next = { static_cast<float_t>(xNext), static_cast<float_t>(yNext), m_player.w, m_player.h };
	//			if (hitTest(next, 0.0))
	//				continue;
	//		}
	//
	//		score += pickupPowerScore(next, power);
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
	//		std::cout << "pickupPower()无路可走" << std::endl;
	//
	//	return true;
	//}

	// 处理敌人
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

	// 检测攻击状况
	//bool TH10Bot::checkShootStatus()
	//{
	//	// 无敌人
	//	if (m_enemies.empty())
	//	{
	//		return false;
	//	}
	//
	//	return true;
	//}

	// 攻击敌人
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
	//		if (hitTest(next, 0.0))
	//		{
	//			shift = true;
	//			xNext = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED_SLOW;
	//			yNext = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED_SLOW;
	//			FixPos(xNext, yNext);
	//			next = { static_cast<float_t>(xNext), static_cast<float_t>(yNext), m_player.w, m_player.h };
	//			if (hitTest(next, 0.0))
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
	//		std::cout << "shootEnemy()无路可走" << std::endl;
	//
	//	return true;
	//}

	// 闪避敌人评分
	//float_t TH10Bot::dodgeEnemyScore(const Player& next)
	//{
	//	float_t allScore = 0.0;
	//	for (const Enemy& enemy : m_enemies)
	//	{
	//		float_t score = 0.0;
	//
	//		// 在敌机范围50外 
	//		if (!next.hitTest(enemy, 50.0))
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

	// 归位
	//void TH10Bot::goback()
	//{
	//	// 靠近初始位置了
	//	if (m_player.hitTest(INIT_RECT, 5.0))
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
	//		if (hitTest(player, 0.0))
	//		{
	//			shift = true;
	//			nextX = m_player.x + MOVE_FACTORS[i].x * MOVE_SPEED_SLOW;
	//			nextY = m_player.y + MOVE_FACTORS[i].y * MOVE_SPEED_SLOW;
	//			FixPos(nextX, nextY);
	//			player = { static_cast<float_t>(nextX), static_cast<float_t>(nextY), m_player.w, m_player.h };
	//			if (hitTest(player, 0.0))
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
	//		std::cout << "goback()无路可走" << std::endl;
	//}
}

#include "Th10Hook/Region.h"

#include "Th10Hook/Scene.h"

namespace th
{
	Region::Region(const vec2& pos0, const vec2& size0) :
		Entity(pos0, vec2(), size0)
	{
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Region::split(int_t times)
	{
		if (times <= 0)
			return;

		if (size.x > size.y)
		{
			m_first = std::make_unique<Region>(vec2(pos.x - size.x / 4, pos.y), vec2(size.x / 2, size.y));
			m_second = std::make_unique<Region>(vec2(pos.x + size.x / 4, pos.y), vec2(size.x / 2, size.y));
		}
		else
		{
			m_first = std::make_unique<Region>(vec2(pos.x, pos.y - size.y / 4), vec2(size.x, size.y / 2));
			m_second = std::make_unique<Region>(vec2(pos.x, pos.y + size.y / 4), vec2(size.x, size.y / 2));
		}

		m_first->split(times - 1);
		m_second->split(times - 1);
	}

	void Region::clearAll()
	{
		m_enemies.clear();
		m_bullets.clear();
		m_lasers.clear();

		if (m_first != nullptr)
			m_first->clearAll();
		if (m_second != nullptr)
			m_second->clearAll();
	}

	void Region::splitEnemies(const std::vector<Enemy>& enemies)
	{
		for (const Enemy& enemy : enemies)
		{
			if (enemy.collide(*this))
			{
				m_enemies.push_back(enemy);
			}
			else
			{
				if (enemy.willCollideWith(*this))
					m_enemies.push_back(enemy);
			}
		}
		if (m_enemies.empty())
			return;

		if (m_first != nullptr)
			m_first->splitEnemies(m_enemies);
		if (m_second != nullptr)
			m_second->splitEnemies(m_enemies);
	}

	void Region::splitBullets(const std::vector<Bullet>& bullets)
	{
		for (const Bullet& bullet : bullets)
		{
			if (bullet.collide(*this))
			{
				m_bullets.push_back(bullet);
			}
			else
			{
				if (bullet.willCollideWith(*this))
					m_bullets.push_back(bullet);
			}
		}
		if (m_bullets.empty())
			return;

		if (m_first != nullptr)
			m_first->splitBullets(m_bullets);
		if (m_second != nullptr)
			m_second->splitBullets(m_bullets);
	}

	void Region::splitLasers(const std::vector<Laser>& lasers)
	{
		for (const Laser& laser : lasers)
		{
			if (laser.collide(*this))
			{
				m_lasers.push_back(laser);
			}
			else
			{
				if (laser.willCollideWith(*this))
					m_lasers.push_back(laser);
			}
		}
		if (m_lasers.empty())
			return;

		if (m_first != nullptr)
			m_first->splitLasers(m_lasers);
		if (m_second != nullptr)
			m_second->splitLasers(m_lasers);
	}

	RegionCollideResult Region::collideAll(const Player& player, float_t frame) const
	{
		RegionCollideResult result = {};
		//result.minCollideFrame = std::numeric_limits<float_t>::max();
		//result.minDistance = std::numeric_limits<float_t>::max();

		if (!collide(player))
			return result;

		// 只检测叶子节点
		if (m_first == nullptr && m_second == nullptr)
		{
			for (Enemy enemy : m_enemies)
			{
				enemy.advance(frame);
				if (enemy.collide(player))
				{
					result.collided = true;
					break;
				}
				//float_t distance = enemy.calcDistance(player.getPosition());
				//if (distance < result.minDistance)
				//	result.minDistance = distance;
				//std::pair<bool, float_t> ret = temp.willCollideWith(player);
				//if (ret.first && ret.second < 2)
				//	return true;
			}

			if (!result.collided)
			{
				for (Bullet bullet : m_bullets)
				{
					bullet.advance(frame);
					if (bullet.collide(player))
					{
						result.collided = true;
						break;
						//if (frame == 0)
						//{
						//	cout << player.x << " " << player.y << " " << player.width << " " << player.height << endl;
						//	cout << bullet.x << " " << bullet.y << " " << bullet.width << " " << bullet.height << endl;
						//}
					}
					//bullet.advance(1);
					//if (bullet.collide(player))
					//{
					//	result.collided = true;
					//	break;
					//}
					//float_t distance = bullet.calcDistance(player.getPosition());
					//if (distance < result.minDistance)
					//	result.minDistance = distance;
					//if (player.collide(temp, 0))
					//{
					//	result.collided = true;
					//	break;
					//}
					//else
					//{
						//std::pair<bool, float_t> ret = temp.willCollideWith(player);
						//if (ret.first && ret.second > -1 && ret.second < 1)
						//{
						//	result.collided = true;
						//	break;
						//}
						//if (ret.first && ret.second > 0 && ret.second < 10)
						//{
						//	result.willCollideCount += 1;
						//	if (ret.second < result.minCollideFrame)
						//		result.minCollideFrame = ret.second;
						//}
					//}
				}
			}

			if (!result.collided)
			{
				for (Laser laser : m_lasers)
				{
					laser.advance(frame);
					if (laser.collide(player))
					{
						result.collided = true;
						break;
					}
					//float_t distance = laser.calcDistance(player.getPosition());
					//if (distance < result.minDistance)
					//	result.minDistance = distance;
					//std::pair<bool, float_t> ret = temp.willCollideWith(player);
					//if (ret.first && ret.second < 2)
					//	return true;
				}
			}
		}

		if (m_first != nullptr)
		{
			RegionCollideResult firstResult = m_first->collideAll(player, frame);
			if (firstResult.collided)
				result.collided = true;
			//if (firstResult.willCollideCount > 0)
			//{
			//	result.willCollideCount += firstResult.willCollideCount;
			//	if (firstResult.minCollideFrame < result.minCollideFrame)
			//		result.minCollideFrame = firstResult.minCollideFrame;
			//}
			//if (firstResult.minDistance < result.minDistance)
			//	result.minDistance = firstResult.minDistance;
		}

		if (m_second != nullptr)
		{
			RegionCollideResult secondResult = m_second->collideAll(player, frame);
			if (secondResult.collided)
				result.collided = true;
			//if (secondResult.willCollideCount > 0)
			//{
			//	result.willCollideCount += secondResult.willCollideCount;
			//	if (secondResult.minCollideFrame < result.minCollideFrame)
			//		result.minCollideFrame = secondResult.minCollideFrame;
			//}
			//if (secondResult.minDistance < result.minDistance)
			//	result.minDistance = secondResult.minDistance;
		}

		return result;
	}

#if RENDER
	void Region::render(cv::Mat& mat, const Player& player)
	{
		if (m_first == nullptr && m_second == nullptr)
		{
			cv::Scalar black(0, 0, 0);
			cv::Scalar white(255, 255, 255);
			cv::Scalar red(0, 0, 255);
			cv::Scalar green(0, 255, 0);
			cv::Scalar blue(255, 0, 0);
			cv::Scalar yellow(0, 255, 255);

			{
				vec2 windowPos = Scene::ToWindowPos(getLeftTop());
				cv::Rect rect(int_t(windowPos.x), int_t(windowPos.y), int_t(size.x), int_t(size.y));
				cv::rectangle(mat, rect, yellow);
			}

			if (collide(player))
			{
				for (const Bullet& bullet : m_bullets)
				{
					vec2 footPoint = bullet.getFootPoint(player);
					vec2 wp1 = Scene::ToWindowPos(player.pos);
					vec2 wp2 = Scene::ToWindowPos(bullet.pos);
					vec2 wp3 = Scene::ToWindowPos(footPoint);
					cv::line(mat, cv::Point(int_t(wp1.x), int_t(wp1.y)), cv::Point(int_t(wp3.x), int_t(wp3.y)), green);
					cv::line(mat, cv::Point(int_t(wp2.x), int_t(wp2.y)), cv::Point(int_t(wp3.x), int_t(wp3.y)), green);
				}

				for (const Laser& laser : m_lasers)
				{
					//vec2 windowPos = Scene::ToWindowPos(laser.getLeftTop());
					//cv::Rect rect(int_t(windowPos.x), int_t(windowPos.y), int_t(laser.size.x), int_t(laser.size.y));
					//cv::rectangle(mat, rect, red);

					vec2 p1 = Scene::ToWindowPos(laser.leftTop);
					vec2 p2 = Scene::ToWindowPos(laser.rightTop);
					vec2 p3 = Scene::ToWindowPos(laser.rightBottom);
					vec2 p4 = Scene::ToWindowPos(laser.leftBottom);
					cv::line(mat, cv::Point(int_t(p1.x), int_t(p1.y)), cv::Point(int_t(p2.x), int_t(p2.y)), red);
					cv::line(mat, cv::Point(int_t(p2.x), int_t(p2.y)), cv::Point(int_t(p3.x), int_t(p3.y)), red);
					cv::line(mat, cv::Point(int_t(p3.x), int_t(p3.y)), cv::Point(int_t(p4.x), int_t(p4.y)), red);
					cv::line(mat, cv::Point(int_t(p4.x), int_t(p4.y)), cv::Point(int_t(p1.x), int_t(p1.y)), red);

					//vec2 o = Scene::ToWindowPos(vec2(0, 0));
					//vec2 axisX = o + laser.axisX * 100;
					//vec2 axisY = o + laser.axisY * 100;
					//cv::line(mat, cv::Point(int_t(o.x), int_t(o.y)), cv::Point(int_t(axisX.x), int_t(axisX.y)), green);
					//cv::line(mat, cv::Point(int_t(o.x), int_t(o.y)), cv::Point(int_t(axisY.x), int_t(axisY.y)), green);

					//break;
				}
			}
		}

		if (m_first != nullptr)
			m_first->render(mat, player);
		if (m_second != nullptr)
			m_second->render(mat, player);
	}
#endif
}

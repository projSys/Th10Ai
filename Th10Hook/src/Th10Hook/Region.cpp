#include "Th10Hook/Region.h"

#include "Th10Hook/Scene.h"

namespace th
{
	Region::Region(const vec2& pos0, const vec2& size0) :
		Entity(pos0, vec2(), size0),
		m_times(0)
	{
		aabb.update(*this);

		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Region::split(int_t times)
	{
		m_times = times;
		if (times <= 0)
			return;

		if (size.x > size.y)
		{
			m_first = std::make_unique<Region>(vec2(pos.x - size.x / _F(4.0), pos.y), vec2(size.x / _F(2.0), size.y));
			m_second = std::make_unique<Region>(vec2(pos.x + size.x / _F(4.0), pos.y), vec2(size.x / _F(2.0), size.y));
		}
		else
		{
			m_first = std::make_unique<Region>(vec2(pos.x, pos.y - size.y / _F(4.0)), vec2(size.x, size.y / _F(2.0)));
			m_second = std::make_unique<Region>(vec2(pos.x, pos.y + size.y / _F(4.0)), vec2(size.x, size.y / _F(2.0)));
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
			if (enemy.willCollideWith(*this))
				m_enemies.push_back(enemy);
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
			if (bullet.willCollideWith(*this))// || bullet.collide(*this))
				m_bullets.push_back(bullet);
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
			if (laser.willCollideWith(*this))
				m_lasers.push_back(laser);
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
					}
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
				}
			}
		}

		if (m_first != nullptr)
		{
			RegionCollideResult firstResult = m_first->collideAll(player, frame);
			if (firstResult.collided)
				result.collided = true;
		}

		if (m_second != nullptr)
		{
			RegionCollideResult secondResult = m_second->collideAll(player, frame);
			if (secondResult.collided)
				result.collided = true;
		}

		return result;
	}

	RegionCollideResult Region::collideAll(const Player& player, float_t frame, const Bullet& target) const
	{
		RegionCollideResult result = {};

		std::cout << m_times << " collideAll size: " << m_bullets.size() << std::endl;
		if (target.willCollideWith(*this))
		{
			std::cout << m_times << " willCollideWith" << std::endl;
		}
		for (const Bullet& bullet : m_bullets)
		{
			if (bullet.id == target.id)
			{
				std::cout << m_times << " have target" << std::endl;
			}
		}

		if (!collide(player))
		{
			return result;
		}

		// 只检测叶子节点
		if (m_first == nullptr && m_second == nullptr)
		{
			//for (Enemy enemy : m_enemies)
			//{
			//	enemy.advance(frame);
			//	if (enemy.collide(player))
			//	{
			//		result.collided = true;
			//		break;
			//	}
			//}

			//if (!result.collided)
			//{
			//	std::cout << m_times << " collide Bullet size: " << m_bullets.size() << std::endl;
			//	for (const Bullet& bullet : m_bullets)
			//	{
			//		if (bullet.id == target.id)
			//			std::cout << m_times << " collide Bullet true" << std::endl;
			//		else
			//			std::cout << m_times << " collide Bullet false" << std::endl;
			//	}
			//}

			//if (!result.collided)
			//{
			//	for (Laser laser : m_lasers)
			//	{
			//		laser.advance(frame);
			//		if (laser.collide(player))
			//		{
			//			result.collided = true;
			//			break;
			//		}
			//	}
			//}
		}

		if (m_first != nullptr)
		{
			RegionCollideResult firstResult = m_first->collideAll(player, frame, target);
			if (firstResult.collided)
				result.collided = true;
		}

		if (m_second != nullptr)
		{
			RegionCollideResult secondResult = m_second->collideAll(player, frame, target);
			if (secondResult.collided)
				result.collided = true;
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

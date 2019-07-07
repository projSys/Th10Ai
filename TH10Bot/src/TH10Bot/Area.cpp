#include "TH10Bot/Common.h"
#include "TH10Bot/Area.h"

#include "TH10Bot/Scene.h"

namespace th
{
	Area::Area(float_t x0, float_t y0, float_t width0, float_t height0) :
		Entity(x0, y0, 0.0f, 0.0f, width0, height0)
	{
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Area::split(int_t times)
	{
		if (times <= 0)
			return;

		if (width > height)
		{
			m_first = std::make_shared<Area>(x - width / 4.0f, y, width / 2.0f, height);
			m_second = std::make_shared<Area>(x + width / 4.0f, y, width / 2.0f, height);
		}
		else
		{
			m_first = std::make_shared<Area>(x, y - height / 4.0f, width, height / 2.0f);
			m_second = std::make_shared<Area>(x, y + height / 4.0f, width, height / 2.0f);
		}

		m_first->split(times - 1);
		m_second->split(times - 1);
	}

	void Area::clearAll()
	{
		m_enemies.clear();
		m_bullets.clear();
		m_lasers.clear();

		if (m_first != nullptr)
			m_first->clearAll();
		if (m_second != nullptr)
			m_second->clearAll();
	}

	void Area::splitEnemies(const std::vector<Enemy>& enemies)
	{
		for (const Enemy& enemy : enemies)
		{
			if (enemy.collide(*this))
			{
				m_enemies.push_back(enemy);
			}
			else
			{
				std::pair<bool, float_t> ret = enemy.willCollideWith(*this);
				if (ret.first && ret.second >= 0.0f && ret.second <= 600.0f)
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

	void Area::splitBullets(const std::vector<Bullet>& bullets)
	{
		for (const Bullet& bullet : bullets)
		{
			if (bullet.collide(*this))
			{
				m_bullets.push_back(bullet);
			}
			else
			{
				std::pair<bool, float_t> ret = bullet.willCollideWith(*this);
				if (ret.first && ret.second >= 0.0f && ret.second <= 600.0f)
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

	void Area::splitLasers(const std::vector<Laser>& lasers)
	{
		for (const Laser& laser : lasers)
		{
			if (laser.collide(*this))
			{
				m_lasers.push_back(laser);
			}
			else
			{
				std::pair<bool, float_t> ret = laser.willCollideWith(*this);
				if (ret.first && ret.second >= 0.0f && ret.second <= 600.0f)
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

	bool Area::collideAll(const Player& player, float_t frame) const
	{
		if (!collide(player))
			return false;

		// 只检测叶子节点
		if (m_first == nullptr && m_second == nullptr)
		{
			for (const Enemy& enemy : m_enemies)
			{
				Enemy temp = enemy;
				temp.advance(frame);
				if (temp.collide(player))
					return true;
				//std::pair<bool, float_t> ret = temp.willCollideWith(player);
				//if (ret.first && ret.second < 2.0f)
				//	return true;
			}

			//int_t count = 0;
			for (const Bullet& bullet : m_bullets)
			{
				Bullet temp = bullet;
				temp.advance(frame);
				if (temp.collide(player))
					return true;
				//std::pair<bool, float_t> ret = temp.willCollideWith(player);
				//if (ret.first)
				//{
				//	//if (ret.second < 3.0f)
				//	//	return true;
				//	//else
				//		++count;
				//}
			}
			//if (count > 20)
			//	return true;

			for (const Laser& laser : m_lasers)
			{
				Laser temp = laser;
				temp.advance(frame);
				if (temp.collide(player))
					return true;
				//std::pair<bool, float_t> ret = temp.willCollideWith(player);
				//if (ret.first && ret.second < 2.0f)
				//	return true;
			}
		}

		if (m_first != nullptr)
		{
			if (m_first->collideAll(player, frame))
				return true;
		}
		if (m_second != nullptr)
		{
			if (m_second->collideAll(player, frame))
				return true;
		}
		return false;
	}

	void Area::render(cv::Mat& buffer, const Player& player)
	{
		if (m_first == nullptr && m_second == nullptr)
		{
			Pointi pos = Scene::ToWindowPos(getTopLeft());
			cv::rectangle(buffer, cv::Rect(pos.x, pos.y,
				static_cast<int_t>(std::round(width)), static_cast<int_t>(std::round(height))),
				cv::Scalar(0, 255, 0));

			if (collide(player))
			{
				for (const Enemy& enemy : m_enemies)
				{
					Pointi windowPos = Scene::ToWindowPos(enemy.getTopLeft());
					cv::Rect rect(windowPos.x, windowPos.y, int_t(enemy.width), int_t(enemy.height));
					cv::rectangle(buffer, rect, cv::Scalar(0, 0, 255));
				}

				for (const Bullet& bullet : m_bullets)
				{
					Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
					cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
					cv::rectangle(buffer, rect, cv::Scalar(0, 0, 255));
				}

				for (const Laser& laser : m_lasers)
				{
					LaserBox laserBox(laser);
					Pointi p1 = Scene::ToWindowPos(laserBox.topLeft);
					Pointi p2 = Scene::ToWindowPos(laserBox.topRight);
					Pointi p3 = Scene::ToWindowPos(laserBox.bottomRight);
					Pointi p4 = Scene::ToWindowPos(laserBox.bottomLeft);
					cv::line(buffer, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), cv::Scalar(0, 0, 255));
					cv::line(buffer, cv::Point(p2.x, p2.y), cv::Point(p3.x, p3.y), cv::Scalar(0, 0, 255));
					cv::line(buffer, cv::Point(p3.x, p3.y), cv::Point(p4.x, p4.y), cv::Scalar(0, 0, 255));
					cv::line(buffer, cv::Point(p4.x, p4.y), cv::Point(p1.x, p1.y), cv::Scalar(0, 0, 255));
				}
			}
		}

		if (m_first != nullptr)
			m_first->render(buffer, player);
		if (m_second != nullptr)
			m_second->render(buffer, player);
	}
}

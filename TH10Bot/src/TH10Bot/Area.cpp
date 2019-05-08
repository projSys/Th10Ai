#include "TH10Bot/Common.h"
#include "TH10Bot/Area.h"

#include "TH10Bot/Scene.h"

namespace th
{
	Area::Area()
	{
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Area::split(int_t times)
	{
		if (times <= 0)
			return;

		m_first = std::make_shared<Area>();
		m_second = std::make_shared<Area>();
		if (width > height)
		{
			m_first->x = x - width / 4.0f;
			m_first->y = y;
			m_first->dx = dx;
			m_first->dy = dy;
			m_first->width = width / 2.0f;
			m_first->height = height;

			m_second->x = x + width / 4.0f;
			m_second->y = y;
			m_second->dx = dx;
			m_second->dy = dy;
			m_second->width = width / 2.0f;
			m_second->height = height;
		}
		else
		{
			m_first->x = x;
			m_first->y = y - height / 4.0f;
			m_first->dx = dx;
			m_first->dy = dy;
			m_first->width = width;
			m_first->height = height / 2.0f;

			m_second->x = x;
			m_second->y = y + height / 4.0f;
			m_second->dx = dx;
			m_second->dy = dy;
			m_second->width = width;
			m_second->height = height / 2.0f;
		}
		m_first->split(times - 1);
		m_second->split(times - 1);
	}

	void Area::clear()
	{
		m_enemies.clear();
		m_bullets.clear();
		m_lasers.clear();

		if (m_first != nullptr)
			m_first->clear();
		if (m_second != nullptr)
			m_second->clear();
	}

	void Area::splitEnemies(const std::vector<Enemy>& enemies)
	{
		for (const Enemy& enemy : enemies)
		{
			if (enemy.willCollideWith(*this).first)
				m_enemies.push_back(enemy);
		}
		if (m_enemies.empty())
			return;
#ifdef _DEBUG
		if (m_enemies.size() > 200)
			std::cout << "Enemy " << m_enemies.size() << std::endl;
#endif

		if (m_first != nullptr)
			m_first->splitEnemies(m_enemies);
		if (m_second != nullptr)
			m_second->splitEnemies(m_enemies);
	}

	void Area::splitBullets(const std::vector<Bullet>& bullets)
	{
		for (const Bullet& bullet : bullets)
		{
			if (bullet.willCollideWith(*this).first)
				m_bullets.push_back(bullet);
		}
		if (m_bullets.empty())
			return;
#ifdef _DEBUG
		if (m_bullets.size() > 500)
			std::cout << "Bullet " << m_bullets.size() << std::endl;
#endif

		if (m_first != nullptr)
			m_first->splitBullets(m_bullets);
		if (m_second != nullptr)
			m_second->splitBullets(m_bullets);
	}

	void Area::splitLasers(const std::vector<Laser>& lasers)
	{
		for (const Laser& laser : lasers)
		{
			if (laser.willCollideWith(*this).first)
				m_lasers.push_back(laser);
		}
		if (m_lasers.empty())
			return;
#ifdef _DEBUG
		if (m_lasers.size() > 200)
			std::cout << "Laser " << m_lasers.size() << std::endl;
#endif

		if (m_first != nullptr)
			m_first->splitLasers(m_lasers);
		if (m_second != nullptr)
			m_second->splitLasers(m_lasers);
	}

	void Area::renderTo(cv::Mat& buffer, const Player& player)
	{
		Pointi pos = Scene::ToWindowPos(getTopLeft());
		cv::rectangle(buffer, cv::Rect(pos.x, pos.y,
			static_cast<int_t>(std::round(width)), static_cast<int_t>(std::round(height))),
			cv::Scalar(0, 255, 0));

		if (collide(player))
		{
			bool b1 = false, b2 = false;
			if (m_first != nullptr)
				b1 = m_first->collide(player);
			if (m_second != nullptr)
				b2 = m_second->collide(player);
			if ((b1 && b2) || (m_first == nullptr && m_second == nullptr))
			{
				for (const Bullet& bullet : m_bullets)
				{
					Pointi windowPos = Scene::ToWindowPos(bullet.getTopLeft());
					cv::Rect rect(windowPos.x, windowPos.y, int_t(bullet.width), int_t(bullet.height));
					cv::rectangle(buffer, rect, cv::Scalar(0, 0, 255), -1);
				}
			}
		}

		if (m_first != nullptr)
			m_first->renderTo(buffer, player);
		if (m_second != nullptr)
			m_second->renderTo(buffer, player);
	}
}

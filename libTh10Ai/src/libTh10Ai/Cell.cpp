#include "libTh10Ai/Common.h"
#include "libTh10Ai/Cell.h"

#include "libTh10Ai/Scene.h"

namespace th
{
	Cell::Cell(float_t x0, float_t y0, float_t width0, float_t height0) :
		Entity(x0, y0, 0.0f, 0.0f, width0, height0)
	{
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Cell::split(int_t times)
	{
		if (times <= 0)
			return;

		if (width > height)
		{
			m_first = std::make_shared<Cell>(x - width / 4.0f, y, width / 2.0f, height);
			m_second = std::make_shared<Cell>(x + width / 4.0f, y, width / 2.0f, height);
		}
		else
		{
			m_first = std::make_shared<Cell>(x, y - height / 4.0f, width, height / 2.0f);
			m_second = std::make_shared<Cell>(x, y + height / 4.0f, width, height / 2.0f);
		}

		m_first->split(times - 1);
		m_second->split(times - 1);
	}

	void Cell::clearAll()
	{
		m_enemies.clear();
		m_bullets.clear();
		m_lasers.clear();

		if (m_first != nullptr)
			m_first->clearAll();
		if (m_second != nullptr)
			m_second->clearAll();
	}

	void Cell::splitEnemies(const std::vector<Enemy>& enemies)
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
				if (ret.first /*&& ret.second >= -60.0f && ret.second <= 600.0f*/)
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

	void Cell::splitBullets(const std::vector<Bullet>& bullets)
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
				if (ret.first /*&& ret.second >= -60.0f && ret.second <= 600.0f*/)
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

	void Cell::splitLasers(const std::vector<Laser>& lasers)
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
				if (ret.first /*&& ret.second >= -60.0f && ret.second <= 600.0f*/)
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

	CellCollideResult Cell::collideAll(const Player& player, float_t frame) const
	{
		CellCollideResult result;
		result.collided = false;
		result.willCollideCount = 0;
		result.minCollideFrame = std::numeric_limits<float_t>::max();

		if (!collide(player))
			return result;

		// 只检测叶子节点
		if (m_first == nullptr && m_second == nullptr)
		{
			for (const Enemy& enemy : m_enemies)
			{
				Enemy temp = enemy;
				temp.advance(frame);
				if (temp.collide(player))
				{
					result.collided = true;
					break;
				}
				//std::pair<bool, float_t> ret = temp.willCollideWith(player);
				//if (ret.first && ret.second < 2.0f)
				//	return true;
			}

			if (!result.collided)
			for (const Bullet& bullet : m_bullets)
			{
				Bullet temp = bullet;
				temp.advance(frame);
				if (temp.collide(player))
				{
					result.collided = true;
					break;
				}
				else
				{
					std::pair<bool, float_t> ret = temp.willCollideWith(player);
					if (ret.first && ret.second > -1.0f && ret.second <= 1.0f)
					{
						result.collided = true;
						break;
					}
					//if (ret.first && ret.second > 0.0f && ret.second < 10.0f)
					//{
					//	result.willCollideCount += 1;
					//	if (ret.second < result.minCollideFrame)
					//		result.minCollideFrame = ret.second;
					//}
				}
			}

			if (!result.collided)
			for (const Laser& laser : m_lasers)
			{
				Laser temp = laser;
				temp.advance(frame);
				if (temp.collide(player))
				{
					result.collided = true;
					break;
				}
				//std::pair<bool, float_t> ret = temp.willCollideWith(player);
				//if (ret.first && ret.second < 2.0f)
				//	return true;
			}
		}

		if (m_first != nullptr)
		{
			CellCollideResult firstResult = m_first->collideAll(player, frame);
			if (firstResult.collided)
				result.collided = true;
			if (firstResult.willCollideCount > 0)
			{
				result.willCollideCount += firstResult.willCollideCount;
				if (firstResult.minCollideFrame < result.minCollideFrame)
					result.minCollideFrame = firstResult.minCollideFrame;
			}
		}

		if (m_second != nullptr)
		{
			CellCollideResult secondResult = m_second->collideAll(player, frame);
			if (secondResult.collided)
				result.collided = true;
			if (secondResult.willCollideCount > 0)
			{
				result.willCollideCount += secondResult.willCollideCount;
				if (secondResult.minCollideFrame < result.minCollideFrame)
					result.minCollideFrame = secondResult.minCollideFrame;
			}
		}

		return result;
	}
}

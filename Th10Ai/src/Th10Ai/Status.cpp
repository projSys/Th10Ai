#include "Th10Ai/Status.h"

#include <Base/Clock.h>

#include "Th10Ai/Scene.h"

namespace th
{
	Status::Status() :
		m_findItemTime(0)
	{
		m_items.reserve(2000);
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Status::update(const StatusData& data)
	{
		m_player = Player(data.player);
	}

	// 查找道具
	boost::optional<Item> Status::findItem()
	{
		boost::optional<Item> target;

		if (m_items.empty())
			return target;

		Clock clock;
		clock.update();

		// 拾取冷却中
		if (clock.getTime() - m_findItemTime < 3000)
			return target;

		// 自机高于1/4屏
		if (m_player.y < Scene::SIZE.height / 4.0f)
		{
			// 进入冷却
			m_findItemTime = clock.getTime();
			return target;
		}

		// 自机高于1/2屏，道具少于10个，敌人多于5个
		if (m_player.y < Scene::SIZE.height / 2.0f && m_items.size() < 10 && m_enemies.size() > 5)
		{
			// 进入冷却
			m_findItemTime = clock.getTime();
			return target;
		}

		float_t minDist = std::numeric_limits<float_t>::max();
		//float_t maxY = std::numeric_limits<float_t>::lowest();
		for (const Item& item : m_items)
		{
			if (!Scene::IsInScene(item.getPosition()))
				continue;

			// 道具高于1/5屏
			if (item.y < Scene::SIZE.height / 5.0f)
				continue;

			// 道具不在自机1/4屏内
			float_t dy = std::abs(item.y - m_player.y);
			if (dy > Scene::SIZE.height / 4.0f)
				continue;

			// 道具太靠近敌机
			bool tooClose = false;
			for (const Enemy& enemy : m_enemies)
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
			float_t dist = item.calcDistance(m_player.getPosition());
			if (dist < minDist)
			{
				minDist = dist;
				target = item;
			}

			//if (item.y > maxY)
			//{
			//	maxY = item.y;
			//	target = item;
			//}
		}

		return target;
	}

	bool Status::hasEnemy() const
	{
		return !m_enemies.empty();
	}

	bool Status::isBoss() const
	{
		return m_enemies.size() == 1 && m_enemies[0].isBoss();
	}

	bool Status::isTalking() const
	{
		return (m_enemies.empty() || isBoss()) && m_bullets.empty() && m_lasers.empty();
	}

	bool Status::isUnderEnemy() const
	{
		bool underEnemy = false;
		for (const Enemy& enemy : m_enemies)
		{
			if (std::abs(m_player.x - enemy.x) < 16.0f && m_player.y > enemy.y)
			{
				underEnemy = true;
				break;
			}
		}
		return underEnemy;
	}

	// 查找敌人
	boost::optional<Enemy> Status::findEnemy()
	{
		boost::optional<Enemy> target;

		if (m_enemies.empty())
			return target;

		// 自机高于1/4屏
		if (m_player.y < Scene::SIZE.height / 4.0f)
			return target;

		float_t minDist = std::numeric_limits<float_t>::max();
		for (const Enemy& enemy : m_enemies)
		{
			if (!Scene::IsInScene(enemy.getPosition()))
				continue;

			if (enemy.y > m_player.y)
				continue;

			// 与自机X轴距离最近
			float_t dx = std::abs(enemy.x - m_player.x);
			if (dx < minDist)
			{
				minDist = dx;
				target = enemy;
			}
		}

		return target;
	}

	const Player& Status::getPlayer() const
	{
		return m_player;
	}

	const std::vector<Item>& Status::getItems() const
	{
		return m_items;
	}

	const std::vector<Enemy>& Status::getEnemies() const
	{
		return m_enemies;
	}

	const std::vector<Bullet>& Status::getBullets() const
	{
		return m_bullets;
	}

	const std::vector<Laser>& Status::getLasers() const
	{
		return m_lasers;
	}
}

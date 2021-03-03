#include "Th10Hook/Status.h"

#include <Base/Time.h>

#include "Th10Hook/Scene.h"

namespace th
{
	Status::Status() :
		m_inputFrame(1),
		m_presentFrame(0),
		m_findItemTime(0)
	{
		m_items.reserve(2000);
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Status::update(const StatusData& data)
	{
		m_inputFrame = data.inputFrame;
		m_presentFrame = data.presentFrame;
		m_player = Player(data.player);
		m_items.clear();
		for (uint_t i = 0; i < data.itemCount; ++i)
			m_items.emplace_back(data.items[i]);
		m_enemies.clear();
		for (uint_t i = 0; i < data.enemyCount; ++i)
			m_enemies.emplace_back(data.enemies[i]);
		m_bullets.clear();
		for (uint_t i = 0; i < data.bulletCount; ++i)
			m_bullets.emplace_back(data.bullets[i]);
		m_lasers.clear();
		for (uint_t i = 0; i < data.laserCount; ++i)
			m_lasers.emplace_back(data.lasers[i]);
	}

	void Status::update(const Status& other)
	{
		m_inputFrame = other.m_inputFrame;
		m_presentFrame = other.m_presentFrame;
		m_player = other.m_player;
		m_items.clear();
		for (const Item& item : other.m_items)
			m_items.push_back(item);
		m_enemies.clear();
		for (const Enemy& enemy : other.m_enemies)
			m_enemies.push_back(enemy);
		m_bullets.clear();
		for (const Bullet& bullet : other.m_bullets)
			m_bullets.push_back(bullet);
		m_lasers.clear();
		for (const Laser& laser : other.m_lasers)
			m_lasers.push_back(laser);
	}

	// 查找道具
	boost::optional<Item> Status::findItem()
	{
		boost::optional<Item> target;

		if (m_items.empty())
			return target;

		Time time = Time::Now();
		int64_t now = time.getMilliSeconds();

		// 拾取冷却中
		if (now - m_findItemTime < 3000)
			return target;

		// 自机高于1/4屏
		if (m_player.y < Scene::SIZE.height / 4.0f)
		{
			// 进入冷却
			m_findItemTime = now;
			return target;
		}

		// 自机高于1/2屏，道具少于10个，敌人多于5个
		if (m_player.y < Scene::SIZE.height / 2.0f && m_items.size() < 10 && m_enemies.size() > 5)
		{
			// 进入冷却
			m_findItemTime = now;
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

	int_t Status::collide(const Player& player, float_t frame) const
	{
		for (const Bullet& org : m_bullets)
		{
			Bullet bullet = org;
			bullet.advance(frame);
			if (bullet.collide(player))
			{
				std::cout << m_inputFrame << "/" << m_presentFrame << "帧" << " 总数：" << m_bullets.size() << " 碰撞："
					<< "org(" << org.id << " " << org.x << " " << org.y << " " << org.dx << " " << org.dy << ") "
					<< "now(" << bullet.id << " " << bullet.x << " " << bullet.y << " " << bullet.dx << " " << bullet.dy << ") " << std::endl;
				return bullet.id;
			}
		}
		std::cout << m_inputFrame << "/" << m_presentFrame << "帧 不碰撞" << " 总数：" << m_bullets.size() << std::endl;
		return -1;
	}

	int_t Status::collide(const Player& player, float_t frame, int_t id) const
	{
		for (const Bullet& org : m_bullets)
		{
			if (org.id == id)
			{
				Bullet bullet = org;
				bullet.advance(frame);
				if (bullet.collide(player))
				{
					std::cout << m_inputFrame << "/" << m_presentFrame << "帧" << " 总数：" << m_bullets.size() << " 碰撞："
						<< "org(" << org.id << " " << org.x << " " << org.y << " " << org.dx << " " << org.dy << ") "
						<< "now(" << bullet.id << " " << bullet.x << " " << bullet.y << " " << bullet.dx << " " << bullet.dy << ") " << std::endl;
					return bullet.id;
				}
			}
		}
		std::cout << m_inputFrame << "/" << m_presentFrame << "帧" << " 找不到子弹：" << id << " 总数：" << m_bullets.size() << std::endl;
		return -1;
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

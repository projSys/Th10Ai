#include "libTH10Bot/Common.h"
#include "libTH10Bot/Data.h"

#include "libTH10Bot/Api.h"
#include "libTH10Bot/Scene.h"

namespace th
{
	Data::Data(Api& api) :
		m_api(api)
	{
		m_items.reserve(200);
		m_enemies.reserve(200);
		m_bullets.reserve(2000);
		m_lasers.reserve(200);
	}

	void Data::update()
	{
		m_api.readPlayer(m_player);
		m_api.readItems(m_items);
		m_api.readEnemies(m_enemies);
		m_api.readBullets(m_bullets);
		m_api.readLasers(m_lasers);
	}

	void Data::checkPrevMove(Direction dir, bool slow)
	{
		m_player.checkPrevMove(dir, slow);
	}

	bool Data::isRebirthStatus() const
	{
		return m_player.isRebirthStatus();
	}

	bool Data::isNormalStatus() const
	{
		return m_player.isNormalStatus();
	}

	bool Data::isColliding() const
	{
		return m_player.isColliding();
	}

	bool Data::isInvincible() const
	{
		return m_player.isInvincible();
	}

	// 查找道具
	//int_t Data::findItem()
	//{
	//	int_t id = -1;

	//	if (m_items.empty())
	//		return id;

	//	// 拾取冷却中
	//	if (m_clock.getTimestamp() - m_collectCooldown < 3000)
	//		return id;

	//	// 自机高于1/4屏
	//	if (m_player.y < SCENE_SIZE.height / 4.0f)
	//	{
	//		// 进入冷却
	//		m_collectCooldown = m_clock.getTimestamp();
	//		return id;
	//	}

	//	// 自机高于1/2屏，道具少于10个，敌人多于5个
	//	if (m_player.y < SCENE_SIZE.height / 2.0f && m_items.size() < 10 && m_enemies.size() > 5)
	//	{
	//		// 进入冷却
	//		m_collectCooldown = m_clock.getTimestamp();
	//		return id;
	//	}

	//	float_t minDist = std::numeric_limits<float_t>::max();
	//	for (uint_t i = 0; i < m_items.size(); ++i)
	//	{
	//		const Item& item = m_items[i];

	//		// 道具高于1/5屏
	//		if (item.y < SCENE_SIZE.height / 5.0f)
	//			continue;

	//		// 道具不在自机1/3屏内
	//		float_t dy = std::abs(item.y - m_player.y);
	//		if (dy > SCENE_SIZE.height / 3.0f)
	//			continue;

	//		// 道具太靠近敌机
	//		bool tooClose = false;
	//		for (const Enemy& enemy : m_enemies)
	//		{
	//			if (item.calcDistance(enemy.getPosition()) < 150.0f)
	//			{
	//				tooClose = true;
	//				break;
	//			}
	//		}
	//		if (tooClose)
	//			continue;

	//		// 道具与自机距离最近
	//		float_t dist = item.calcDistance(m_player.getPosition());
	//		if (dist < minDist)
	//		{
	//			minDist = dist;
	//			id = i;
	//		}
	//	}

	//	return id;
	//}

	bool Data::hasEnemy() const
	{
		return !m_enemies.empty();
	}

	bool Data::isTalking() const
	{
		return m_enemies.size() <= 1 && m_bullets.empty() && m_lasers.empty();
	}

	bool Data::isUnderEnemy() const
	{
		bool underEnemy = false;
		for (const Enemy& enemy : m_enemies)
		{
			if (std::abs(m_player.x - enemy.x) < 20.0f && m_player.y > enemy.y)
			{
				underEnemy = true;
				break;
			}
		}
		return underEnemy;
	}

	const Player& Data::getPlayer() const
	{
		return m_player;
	}

	const std::vector<Item>& Data::getItems() const
	{
		return m_items;
	}

	const std::vector<Enemy>& Data::getEnemies() const
	{
		return m_enemies;
	}

	const std::vector<Bullet>& Data::getBullets() const
	{
		return m_bullets;
	}

	const std::vector<Laser>& Data::getLasers() const
	{
		return m_lasers;
	}
}

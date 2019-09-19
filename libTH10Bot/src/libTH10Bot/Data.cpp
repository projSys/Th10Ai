#include "libTH10Bot/Common.h"
#include "libTH10Bot/Data.h"

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
		if (!m_api.readPlayer(m_player))
			std::cout << "¶ÁÈ¡PlayerÊ§°Ü¡£" << std::endl;

		if (!m_api.readItems(m_items))
			std::cout << "¶ÁÈ¡ItemsÊ§°Ü¡£" << std::endl;

		if (!m_api.readEnemies(m_enemies))
			std::cout << "¶ÁÈ¡EnemiesÊ§°Ü¡£" << std::endl;

		if (!m_api.readBullets(m_bullets))
			std::cout << "¶ÁÈ¡BulletsÊ§°Ü¡£" << std::endl;

		if (m_api.readLasers(m_lasers))
			std::cout << "¶ÁÈ¡LasersÊ§°Ü¡£" << std::endl;
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

	bool Data::hasEnemy() const
	{
		return !m_enemies.empty();
	}

	bool Data::isTalking() const
	{
		return m_enemies.size() <= 1 && m_bullets.empty() && m_lasers.empty();
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

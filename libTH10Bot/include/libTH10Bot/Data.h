#pragma once

#include <vector>

#include "libTH10Bot/Entity/Player.h"
#include "libTH10Bot/Entity/Item.h"
#include "libTH10Bot/Entity/Enemy.h"
#include "libTH10Bot/Entity/Bullet.h"
#include "libTH10Bot/Entity/Laser.h"
#include "libTH10Bot/Api.h"

namespace th
{
	class Data
	{
	public:
		Data(Api& api);

		void update();

		// Player
		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;
		bool isInvincible() const;

		// Item
		int_t findItem();

		// Enemy
		bool hasEnemy() const;
		bool isTalking() const;
		bool isUnderEnemy() const;

		const Player& getPlayer() const;
		const std::vector<Item>& getItems() const;
		const std::vector<Enemy>& getEnemies() const;
		const std::vector<Bullet>& getBullets() const;
		const std::vector<Laser>& getLasers() const;

	private:
		Api& m_api;

		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;
	};
}

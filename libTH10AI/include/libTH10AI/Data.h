#pragma once

#include <vector>

#include "libTH10AI/Entity/Player.h"
#include "libTH10AI/Entity/Item.h"
#include "libTH10AI/Entity/Enemy.h"
#include "libTH10AI/Entity/Bullet.h"
#include "libTH10AI/Entity/Laser.h"
#include "libTH10AI/Api.h"

namespace th
{
	struct ItemTarget
	{
		bool valid;
		Item item;
	};

	struct EnemyTarget
	{
		bool valid;
		Enemy enemy;
	};

	class Data
	{
	public:
		Data(Api& api);

		void update();
		void print();

		// Player
		void checkPrevMove(Direction prevDir, bool prevSlow);
		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;
		bool isInvincible() const;

		// Item
		ItemTarget findItem();

		// Enemy
		bool hasEnemy() const;
		bool isBoss() const;
		bool isTalking() const;
		bool isUnderEnemy() const;
		EnemyTarget findEnemy();

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

		std::chrono::steady_clock::time_point m_findItemTime;
	};
}

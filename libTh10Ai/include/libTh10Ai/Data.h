#pragma once

#include <vector>

#include "libTh10Ai/Player.h"
#include "libTh10Ai/Item.h"
#include "libTh10Ai/Enemy.h"
#include "libTh10Ai/Bullet.h"
#include "libTh10Ai/Laser.h"

namespace th
{
	class Reader;

	struct ItemTarget
	{
		bool found;
		Item item;
	};

	struct EnemyTarget
	{
		bool found;
		Enemy enemy;
	};

	class Data
	{
	public:
		Data(Reader& reader);

		void update();
		void print();

		// Player

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
		Reader& m_reader;

		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::chrono::steady_clock::time_point m_findItemTime;
	};
}

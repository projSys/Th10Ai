#pragma once

#include <vector>

#include "Th10Ai/Player.h"
#include "Th10Ai/Item.h"
#include "Th10Ai/Enemy.h"
#include "Th10Ai/Bullet.h"
#include "Th10Ai/Laser.h"

namespace th
{
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
		Data();

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
		const vector<Item>& getItems() const;
		const vector<Enemy>& getEnemies() const;
		const vector<Bullet>& getBullets() const;
		const vector<Laser>& getLasers() const;

	private:
		Player m_player;
		vector<Item> m_items;
		vector<Enemy> m_enemies;
		vector<Bullet> m_bullets;
		vector<Laser> m_lasers;

		std::chrono::steady_clock::time_point m_findItemTime;
	};
}

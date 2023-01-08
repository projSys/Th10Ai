#pragma once

#include "Th10Ai/Common.h"

#include <vector>
#include <optional>

#include "Th10Base/Player.h"
#include "Th10Base/Item.h"
#include "Th10Base/Enemy.h"
#include "Th10Base/Bullet.h"
#include "Th10Base/Laser.h"
#include "Th10Base/SharedStatus.h"

namespace th
{
	class Status
	{
	public:
		Status();

		void clear();
		void update();
		void updateExtra();
		void copy(const Status& other);
		void copy(const SharedStatus& other);

		// Player

		// Item

		// Enemy
		bool haveEnemies() const;
		bool isBoss() const;
		bool isTalking() const;
		bool isUnderEnemy() const;

		// Bullet
		std::optional<Bullet> collide(const Player& player, int_t frame) const;
		int_t collide(const Player& player, int_t frame, int_t id) const;

		// Laser

		const Player& getPlayer() const;
		const std::vector<Item>& getItems() const;
		const std::vector<Enemy>& getEnemies() const;
		const std::vector<Bullet>& getBullets() const;
		const std::vector<Laser>& getLasers() const;

		int_t inputFrame;
		int_t statusFrame;
		int_t handleFrame;
		int_t frame1;

	private:
		GlobalVar m_globalVar;
		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;
	};
}

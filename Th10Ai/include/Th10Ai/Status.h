#pragma once

#include "Th10Ai/Common.h"

#include <vector>
#include <boost/optional.hpp>

#include "Th10Ai/Th10Types.h"
#include "Th10Ai/Player.h"
#include "Th10Ai/Item.h"
#include "Th10Ai/Enemy.h"
#include "Th10Ai/Bullet.h"
#include "Th10Ai/Laser.h"

namespace th
{
	class Status
	{
	public:
		Status();

		void update(const StatusData& data);
		void update(const Status& other);

		// Player

		// Item
		boost::optional<Item> findItem();

		// Enemy
		bool hasEnemy() const;
		bool isBoss() const;
		bool isTalking() const;
		bool isUnderEnemy() const;
		boost::optional<Enemy> findEnemy();

		// Bullet
		int_t collide(const Player& player, float_t frame) const;
		int_t collide(const Player& player, float_t frame, int_t id) const;

		const Player& getPlayer() const;
		const std::vector<Item>& getItems() const;
		const std::vector<Enemy>& getEnemies() const;
		const std::vector<Bullet>& getBullets() const;
		const std::vector<Laser>& getLasers() const;

	private:
		int64_t m_frame;
		Player m_player;
		std::vector<Item> m_items;
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		int64_t m_findItemTime;
	};
}

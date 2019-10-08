#pragma once

#include <memory>

#include "libTh10Ai/Data.h"

namespace th
{
	struct CellCollideResult
	{
		bool collided;
		bool willCollide;
		float_t willCollideFrame;
		int_t willCollideCount;
	};

	class Cell :
		private Entity
	{
	public:
		Cell(float_t x0, float_t y0, float_t width0, float_t height0);

		void split(int_t times);

		void clearAll();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);
		CellCollideResult collideAll(const Player& player, float_t frame) const;

	private:
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::shared_ptr<Cell> m_first;
		std::shared_ptr<Cell> m_second;
	};
}

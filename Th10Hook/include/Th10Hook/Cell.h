#pragma once

#include <memory>

#include "Th10Hook/Data.h"

namespace th
{
	struct CellCollideResult
	{
		bool collided;
		int_t willCollideCount;
		float_t minCollideFrame;
		float_t minDistance;
	};

	class Cell :
		private Entity
	{
	public:
		Cell(float_t x0, float_t y0, float_t width0, float_t height0);

		void split(int_t times);

		void clearAll();
		void splitEnemies(const vector<Enemy>& enemies);
		void splitBullets(const vector<Bullet>& bullets);
		void splitLasers(const vector<Laser>& lasers);
		CellCollideResult collideAll(const Player& player, float_t frame) const;

	private:
		vector<Enemy> m_enemies;
		vector<Bullet> m_bullets;
		vector<Laser> m_lasers;

		shared_ptr<Cell> m_first;
		shared_ptr<Cell> m_second;
	};
}

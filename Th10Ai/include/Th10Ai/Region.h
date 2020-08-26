#pragma once

#include "Th10Ai/Common.h"

#include <memory>

#include "Th10Ai/Status.h"

namespace th
{
	struct RegionCollideResult
	{
		bool collided;
		int_t willCollideCount;
		float_t minCollideFrame;
		float_t minDistance;
	};

	class Region :
		private Entity
	{
	public:
		Region(float_t x0, float_t y0, float_t width0, float_t height0);

		void split(int_t times);

		void clearAll();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);
		RegionCollideResult collideAll(const Player& player, float_t frame) const;

	private:
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::shared_ptr<Region> m_first;
		std::shared_ptr<Region> m_second;
	};
}

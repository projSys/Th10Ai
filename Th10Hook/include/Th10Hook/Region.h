#pragma once

#include "Th10Hook/Common.h"

#include <memory>

#if RENDER
#include <opencv2/opencv.hpp>
#endif

#include "Th10Hook/Entity.h"
#include "Th10Hook/Status.h"

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
		Region(const vec2& pos0, const vec2& size0);

		void split(int_t times);

		void clearAll();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);
		RegionCollideResult collideAll(const Player& player, int_t frame) const;
		RegionCollideResult collideAll(const Player& player, int_t frame, const Bullet& target) const;

#if RENDER
		void render(cv::Mat& mat, const Player& player);
#endif

	private:
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::unique_ptr<Region> m_first;
		std::unique_ptr<Region> m_second;
		int_t m_times;
	};
}

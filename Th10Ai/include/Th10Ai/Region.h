#pragma once

#include "Th10Ai/Common.h"

#include <memory>

#if RENDER
#include <opencv2/opencv.hpp>
#endif

#include "Th10Base/AABB.h"
#include "Th10Base/SharedMemory.h"
#include "Th10Base/SharedData.h"
#include "Th10Base/Player.h"
#include "Th10Base/Item.h"
#include "Th10Base/Enemy.h"
#include "Th10Base/Bullet.h"
#include "Th10Base/Laser.h"
//#include "Th10Ai/Status.h"

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
		public AABB
	{
	public:
		Region(const vec2& pos, const vec2& size);

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
		vec2 getTopLeft() const;
		vec2 getTopRight() const;
		vec2 getBottomLeft() const;
		vec2 getBottomRight() const;
		void updateAABB();

		vec2 m_pos;			// 坐标在中心点
		vec2 m_size;

		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::unique_ptr<Region> m_first;
		std::unique_ptr<Region> m_second;
		int_t m_times;
	};
}

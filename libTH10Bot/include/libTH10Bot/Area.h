#pragma once

#include <vector>
#include <memory>
//#include <opencv2/opencv.hpp>

#include "libTH10Bot/Entity/Entity.h"
#include "libTH10Bot/Entity/Player.h"
#include "libTH10Bot/Entity/Enemy.h"
#include "libTH10Bot/Entity/Bullet.h"
#include "libTH10Bot/Entity/Laser.h"

namespace th
{
	class Area :
		private Entity
	{
	public:
		Area(float_t x0, float_t y0, float_t width0, float_t height0);

		void split(int_t times);

		void clearAll();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);
		bool collideAll(const Player& player, float_t frame) const;

		//void render(cv::Mat& buffer, const Player& player);

	private:
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::shared_ptr<Area> m_first;
		std::shared_ptr<Area> m_second;
	};
}

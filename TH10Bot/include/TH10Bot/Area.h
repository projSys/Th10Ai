#pragma once

#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>

#include "TH10Bot/Entity.h"
#include "TH10Bot/Player.h"
#include "TH10Bot/Enemy.h"
#include "TH10Bot/Bullet.h"
#include "TH10Bot/Laser.h"

namespace th
{
	class Area :
		public Entity
	{
	public:
		Area();

		void split(int_t times);

		void clear();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);

		void renderTo(cv::Mat& buffer, const Player& player);

	private:
		std::vector<Enemy> m_enemies;
		std::vector<Bullet> m_bullets;
		std::vector<Laser> m_lasers;

		std::shared_ptr<Area> m_first;
		std::shared_ptr<Area> m_second;
	};
}

#pragma once

#include "Th10Hook/Common.h"

#if RENDER
#include <opencv2/opencv.hpp>
#endif

#include "Th10Hook/Region.h"

namespace th
{
	// ³¡¾°×ø±ê
	//     +-----------------------+-----------------------+
	//     |      |16                               |      |
	//     |------+----------------+----------------+------|------> X
	//     |  32  |(-192,0)        |(0,0)    (191,0)|      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |448   |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |                |                |      |
	//     |      |(-192,447)      |(0,447)(191,447)|      |
	//     |------+----------------+----------------+------|
	//     |      |               384               |      |
	//     +-----------------------+-----------------------+
	//                             |
	//                            \|/
	//                             Y
	class Scene
	{
	public:
		static constexpr vec2 SIZE = vec2(_F(384.0), _F(448.0));
		static constexpr vec2 OFFSET = vec2(_F(32.0), _F(16.0));
		static constexpr vec2 ORIGIN_POINT_OFFSET = vec2(_F(192.0), _F(0.0));

		static bool IsInScene(const vec2& pos);
		static bool IsInPlayerRegion(const vec2& pos);
		static vec2 ToWindowPos(const vec2& scenePos);
		static vec2 ToScenePos(const vec2& windowPos);

		Scene();

		void clearAll();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);
		RegionCollideResult collideAll(const Player& player, float_t frame) const;
		RegionCollideResult collideAll(const Player& player, float_t frame, const Bullet& target) const;

#if RENDER
		void render(cv::Mat& mat, const Player& player);
#endif

	private:
		Region m_region;
	};
}

#pragma once

#include "Th10Ai/Region.h"

namespace th
{
	// ³¡¾°×ø±ê
	//     +-----------------------+-----------------------+
	//     |      |16                               |      |
	//     |------+----------------+----------------+------|------> X
	//     |  32  |(-192,0)        |(0,0)    (192,0)|      |
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
	//     |      |(-192,448)      |(0,448)(192,448)|      |
	//     |------+----------------+----------------+------|
	//     |      |               384               |      |
	//     +-----------------------+-----------------------+
	//                             |
	//                            \|/
	//                             Y
	class Scene
	{
	public:
		static const Sizef SIZE;

		static bool IsInScene(const Pointf& pos);
		static bool IsInPlayerArea(const Pointf& pos);
		static Pointf FixPlayerPos(const Pointf& pos);
		static Pointi ToWindowPos(const Pointf& scenePos);
		static Pointf ToScenePos(const Pointi& windowPos);

		Scene();

		void split(int_t times);

		void clearAll();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);
		RegionCollideResult collideAll(const Player& player, float_t frame) const;

	private:
		static const Sizef OFFSET;
		static const Sizef ORIGIN_POINT_OFFSET;

		Region m_region;
	};
}

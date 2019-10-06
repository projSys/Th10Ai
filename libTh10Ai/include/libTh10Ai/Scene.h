#pragma once

#include "libTh10Ai/Cell.h"

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
	//     |      |                |448             |      |
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

		static Pointi ToWindowPos(const Pointf& scenePos);
		static Pointf ToScenePos(const Pointi& windowPos);
		static bool IsInScene(const Pointf& pos);
		static bool IsInPlayerArea(const Pointf& pos);
		static Pointf FixPlayerPos(const Pointf& pos);

		Scene();

		void split(int_t times);

		void clearAll();
		void splitEnemies(const std::vector<Enemy>& enemies);
		void splitBullets(const std::vector<Bullet>& bullets);
		void splitLasers(const std::vector<Laser>& lasers);
		CellCollideResult collideAll(const Player& player, float_t frame) const;

	private:
		static const Pointf OFFSET;
		static const Pointf ORIGIN_POINT_OFFSET;

		Cell m_cell;
	};
}

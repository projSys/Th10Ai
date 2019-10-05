#pragma once

#include "libTh10Ai/Cell.h"

namespace th
{
	const Sizef SCENE_SIZE = { 384.0f, 448.0f };

	// ³¡¾°×ø±ê
	//     +-----------------------+-----------------------+
	//     | 24  8|                                 |      |16
	//     |----+-+----------------+----------------+-+----|---------> X
	//   (-200,0) (-192,0)         |(0,0)     (192,0) (200,0)
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |448             | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	// (-200,448) (-192,448)       |(0,448) (192,448) (200,448)
	//     |----+-+----------------+----------------+-+----|
	//     |      |               384               |      |
	//     +-----------------------+-----------------------+
	//                             |
	//                            \|/
	//                             Y
	class Scene
	{
	public:
		static Pointi ToWindowPos(const Pointf& scenePos);
		static Pointf ToScenePos(const Pointi& windowPos);
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
		static const Pointf SCENE_OFFSET;
		static const Pointf ORIGIN_POINT_OFFSET;

		Cell m_cell;
	};
}

#pragma once

#include "libTh10Ai/Data.h"
#include "libTh10Ai/Scene.h"

namespace th
{
	const Direction FIND_DIRS[DIR_MAXCOUNT][DIR_MAXCOUNT] =
	{
		// DIR_HOLD
		{ DIR_HOLD,      DIR_UP,       DIR_DOWN,      DIR_LEFT,    DIR_RIGHT,     DIR_LEFTUP,  DIR_RIGHTUP,  DIR_LEFTDOWN,  DIR_RIGHTDOWN },
		// DIR_UP
		{ DIR_UP,        DIR_LEFTUP,   DIR_RIGHTUP,   DIR_LEFT,    DIR_RIGHT,     DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWN
		{ DIR_DOWN,      DIR_LEFTDOWN, DIR_RIGHTDOWN, DIR_LEFT,    DIR_RIGHT,     DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_LEFT
		{ DIR_LEFT,      DIR_LEFTUP,   DIR_LEFTDOWN,  DIR_UP,      DIR_DOWN,      DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_RIGHT
		{ DIR_RIGHT,     DIR_RIGHTUP,  DIR_RIGHTDOWN, DIR_UP,      DIR_DOWN,      DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPLEFT
		{ DIR_LEFTUP,    DIR_UP,       DIR_LEFT,      DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPRIGHT
		{ DIR_RIGHTUP,   DIR_UP,       DIR_RIGHT,     DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNLEFT
		{ DIR_LEFTDOWN,  DIR_DOWN,     DIR_LEFT,      DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNRIGHT
		{ DIR_RIGHTDOWN, DIR_DOWN,     DIR_RIGHT,     DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_NONE,    DIR_NONE,     DIR_NONE,      DIR_NONE      }
	};

	const int_t FIND_SIZES[DIR_MAXCOUNT] = { 1, 5, 5, 5, 5, 5, 5, 5, 5 };

	struct Action
	{
		Pointf fromPos;
		Direction fromDir;
		bool slowFirst;			// 是否慢速优先
		float_t frame;

		int_t willCollideCount;
		float_t minCollideFrame;
	};

	struct Result
	{
		bool valid;
		bool slow;		// 实际是否慢速
		float_t score;
		int_t size;
	};

	class Path
	{
	public:
		Path(Data& data, Scene& scene,
			ItemTarget& itemTarget, EnemyTarget& enemyTarget, bool underEnemy);

		Result find(Direction dir);

		Result dfs(const Action& action);
		float_t calcCollectScore(const Player& player);
		float_t calcShootScore(const Player& player);
		float_t calcPositionScore(const Player& player);
		float_t calcDepthScore(float_t frame);

		//private:
		static const int_t FIND_LIMIT = 500;	// 500个节点
		static const float_t FIND_DEPTH;

		Data& m_data;
		Scene& m_scene;
		ItemTarget& m_itemTarget;
		EnemyTarget& m_enemyTarget;
		bool m_underEnemy;

		Direction m_dir;

		float_t m_bestScore;
		Direction m_bestDir;
		bool m_bestSlow;
		int_t m_count;
		int_t m_good;
		int_t m_bad;
	};
}

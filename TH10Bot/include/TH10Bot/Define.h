#pragma once

#include <tuple>
#include <map>

namespace th
{
	const Pointf SCENE_POS = { 24.0f, 16.0f };

	const Pointf ORIGIN_POINT_POS = { 200.0f, 0.0f };

	const Pointf PLAYER_INIT_POS = { 0.0f, 400.0f };

	const Sizef SCENE_SIZE = { 384.0f, 448.0f };

	// 方向
	enum Direction
	{
		DIR_NONE = -1,
		DIR_HOLD,			// 不动
		DIR_UP,				// 上
		DIR_DOWN,			// 下
		DIR_LEFT,			// 左
		DIR_RIGHT,			// 右
		DIR_UPLEFT,			// 左上
		DIR_UPRIGHT,		// 右上
		DIR_DOWNLEFT,		// 左下
		DIR_DOWNRIGHT,		// 右下
		DIR_MAXCOUNT
	};

	// 移动速度
	// 3.18 = 4.5 * sqrt(0.5)
	const Pointf MOVE_SPEED[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ 0.0f, -4.5f },
		{ 0.0f, 4.5f },
		{ -4.5f, 0.0f },
		{ 4.5f, 0.0f },
		{ -3.18f, -3.18f },
		{ 3.18f, -3.18f },
		{ -3.18f, 3.18f },
		{ 3.18f, 3.18f }
	};

	// 1.41 = 2.0 * sqrt(0.5)
	const Pointf MOVE_SPEED_SLOW[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ 0.0f, -2.0f },
		{ 0.0f, 2.0f },
		{ -2.0f, 0.0f },
		{ 2.0f, 0.0f },
		{ -1.41f, -1.41f },
		{ 1.41f, -1.41f },
		{ -1.41f, 1.41f },
		{ 1.41f, 1.41f }
	};

	const Direction COLLIDE_PRIORITY[DIR_MAXCOUNT][5] =
	{
		// DIR_HOLD
		{ DIR_NONE,    DIR_NONE,      DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UP
		{ DIR_LEFT,    DIR_RIGHT,     DIR_UPLEFT,   DIR_UPRIGHT,   DIR_UP        },
		// DIR_DOWN
		{ DIR_LEFT,    DIR_RIGHT,     DIR_DOWNLEFT, DIR_DOWNRIGHT, DIR_DOWN      },
		// DIR_LEFT
		{ DIR_UP,      DIR_DOWN,      DIR_UPLEFT,   DIR_DOWNLEFT,  DIR_LEFT      },
		// DIR_RIGHT
		{ DIR_UP,      DIR_DOWN,      DIR_UPRIGHT,  DIR_DOWNRIGHT, DIR_RIGHT     },
		// DIR_UPLEFT
		{ DIR_UPRIGHT, DIR_DOWNLEFT,  DIR_UP,       DIR_LEFT,      DIR_UPLEFT    },
		// DIR_UPRIGHT
		{ DIR_UPLEFT,  DIR_DOWNRIGHT, DIR_UP,       DIR_RIGHT,     DIR_UPRIGHT   },
		// DIR_DOWNLEFT
		{ DIR_UPLEFT,  DIR_DOWNRIGHT, DIR_DOWN,     DIR_LEFT,      DIR_DOWNLEFT  },
		// DIR_DOWNRIGHT
		{ DIR_UPRIGHT, DIR_DOWNLEFT,  DIR_DOWN,     DIR_RIGHT,     DIR_DOWNRIGHT }
	};

	const Direction FROM_PRIORITY[DIR_MAXCOUNT][8] =
	{
		// DIR_HOLD
		{ DIR_UP,        DIR_DOWN,     DIR_LEFT,      DIR_RIGHT,   DIR_UPLEFT,    DIR_UPRIGHT,  DIR_DOWNLEFT,  DIR_DOWNRIGHT },
		// DIR_UP
		{ DIR_UP,        DIR_UPLEFT,   DIR_UPRIGHT,   DIR_LEFT,    DIR_RIGHT,     DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWN
		{ DIR_DOWN,      DIR_DOWNLEFT, DIR_DOWNRIGHT, DIR_LEFT,    DIR_RIGHT,     DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_LEFT
		{ DIR_LEFT,      DIR_UPLEFT,   DIR_DOWNLEFT,  DIR_UP,      DIR_DOWN,      DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_RIGHT
		{ DIR_RIGHT,     DIR_UPRIGHT,  DIR_DOWNRIGHT, DIR_UP,      DIR_DOWN,      DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPLEFT
		{ DIR_UPLEFT,    DIR_UP,       DIR_LEFT,      DIR_UPRIGHT, DIR_DOWNLEFT,  DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPRIGHT
		{ DIR_UPRIGHT,   DIR_UP,       DIR_RIGHT,     DIR_UPLEFT,  DIR_DOWNRIGHT, DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNLEFT
		{ DIR_DOWNLEFT,  DIR_DOWN,     DIR_LEFT,      DIR_UPLEFT,  DIR_DOWNRIGHT, DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNRIGHT
		{ DIR_DOWNRIGHT, DIR_DOWN,     DIR_RIGHT,     DIR_UPRIGHT, DIR_DOWNLEFT,  DIR_NONE,     DIR_NONE,      DIR_NONE      }
	};

	const int_t FROM_END[DIR_MAXCOUNT] = { 8, 5, 5, 5, 5, 5, 5, 5, 5 };

	const float_t PS[DIR_MAXCOUNT] =
	{
		120.0f,
		120.0f,
		110.0f,
		110.0f,
		100.0f,
		90.0f,
		80.0f,
		80.0f,
		70.0f
	};

	struct FootPoint
	{
		float_t x;		// 垂足坐标
		float_t y;
		float_t frame;	// 到达垂足的帧数
	};

	struct BulletView
	{
		uint_t index;
		Direction dir;

		FootPoint footPoint;
	};

	struct LaserLv1
	{
		uint_t index;
		float_t distance;
		FootPoint footPoint;
		float_t angle;
		Direction dir;
	};

	struct Node
	{
		Pointf pos;
		Pointf fromPos;
		Direction fromDir;
		float_t frame;

		float_t gScore;
		float_t hScore;
		float_t fScore;
	};

	struct PointLess
	{
		bool operator ()(const Pointf& left, const Pointf& right) const
		{
			return std::tie(left.x, left.y) < std::tie(right.x, right.y);
		}
	};

	typedef std::map<Pointf, Node, PointLess> PointNodeMap;
	//typedef std::map<float_t, Node> ScoreNodeMap;
	typedef std::multimap<float_t, Node> ScoreNodeMap;	// score可能重复

	struct NodeScore
	{
		float_t score;
		float_t minCollideFrame;
		Direction minCollideDir;
		int_t willCollideCount;
	};

	struct NodeResult
	{
		float_t score;
		Direction nextDir;
		float_t minCollideFrame;
	};
}

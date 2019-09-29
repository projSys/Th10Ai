#pragma once

namespace th
{
	const Direction DODGE_PRIORITY[DIR_MAXCOUNT][5] =
	{
		// DIR_HOLD
		{ DIR_NONE,    DIR_NONE,      DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UP
		{ DIR_LEFT,    DIR_RIGHT,     DIR_LEFTUP,   DIR_RIGHTUP,   DIR_UP        },
		// DIR_DOWN
		{ DIR_LEFT,    DIR_RIGHT,     DIR_LEFTDOWN, DIR_RIGHTDOWN, DIR_DOWN      },
		// DIR_LEFT
		{ DIR_UP,      DIR_DOWN,      DIR_LEFTUP,   DIR_LEFTDOWN,  DIR_LEFT      },
		// DIR_RIGHT
		{ DIR_UP,      DIR_DOWN,      DIR_RIGHTUP,  DIR_RIGHTDOWN, DIR_RIGHT     },
		// DIR_UPLEFT
		{ DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_UP,       DIR_LEFT,      DIR_LEFTUP    },
		// DIR_UPRIGHT
		{ DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_UP,       DIR_RIGHT,     DIR_RIGHTUP   },
		// DIR_DOWNLEFT
		{ DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_DOWN,     DIR_LEFT,      DIR_LEFTDOWN  },
		// DIR_DOWNRIGHT
		{ DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_DOWN,     DIR_RIGHT,     DIR_RIGHTDOWN }
	};

	const Direction MOVE_PRIORITY_DIR[DIR_MAXCOUNT][9] =
	{
		// DIR_HOLD
		{ DIR_HOLD,      DIR_UP,       DIR_DOWN,      DIR_LEFT,    DIR_RIGHT,     DIR_LEFTUP,  DIR_RIGHTUP,  DIR_LEFTDOWN,  DIR_RIGHTDOWN },
		// DIR_UP
		{ DIR_UP,        DIR_LEFTUP,   DIR_RIGHTUP,   DIR_LEFT,    DIR_RIGHT,     DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWN
		{ DIR_DOWN,      DIR_LEFTDOWN, DIR_RIGHTDOWN, DIR_LEFT,    DIR_RIGHT,     DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_LEFT
		{ DIR_LEFT,      DIR_LEFTUP,   DIR_LEFTDOWN,  DIR_UP,      DIR_DOWN,      DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_RIGHT
		{ DIR_RIGHT,     DIR_RIGHTUP,  DIR_RIGHTDOWN, DIR_UP,      DIR_DOWN,      DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPLEFT
		{ DIR_LEFTUP,    DIR_UP,       DIR_LEFT,      DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_UPRIGHT
		{ DIR_RIGHTUP,   DIR_UP,       DIR_RIGHT,     DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNLEFT
		{ DIR_LEFTDOWN,  DIR_DOWN,     DIR_LEFT,      DIR_LEFTUP,  DIR_RIGHTDOWN, DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      },
		// DIR_DOWNRIGHT
		{ DIR_RIGHTDOWN, DIR_DOWN,     DIR_RIGHT,     DIR_RIGHTUP, DIR_LEFTDOWN,  DIR_HOLD,    DIR_NONE,     DIR_NONE,      DIR_NONE      }
	};

	const int_t MOVE_PRIORITY_END[DIR_MAXCOUNT] = { 1, 6, 6, 6, 6, 6, 6, 6, 6 };

	class Mover
	{
	public:
		Mover(Direction targetDir);

		bool hasNext() const;
		Direction next();
		int_t getSize() const;

	private:
		Direction m_targetDir;
		int_t m_i;
	};
}

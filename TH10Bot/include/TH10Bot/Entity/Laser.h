#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	struct Player;

	// ¼¤¹â½á¹¹
	//   (x,y)
	// +---+---+
	// |       |
	// |       |
	// |       |
	// |       | h
	// |       |
	// |       |
	// |       |
	// +-------+
	//     w
	struct Laser :
		Entity
	{
		Laser advance(float_t frame) const;
		bool collide(const Player& player) const;

		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;

		float_t arc;
	};

	struct SATBox
	{
		static Pointf Rotate(const Pointf& P, const Pointf& C, float_t radianC);
		static bool Collide(float_t c1, float_t s1, float_t c2, float_t s2);

		Pointf topLeft;
		Pointf topRight;
		Pointf bottomLeft;
		Pointf bottomRight;
	};

	struct LaserBox :
		SATBox
	{
		LaserBox(const Laser& laser);

		bool collide(const Player& player) const;
	};

	struct PlayerBox :
		SATBox
	{
		PlayerBox(const Player& player, const Laser& laser);

		bool collide(const Laser& laser) const;
	};
}

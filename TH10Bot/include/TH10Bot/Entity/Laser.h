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
		static bool Collide(float_t p1, float_t s1, float_t p2, float_t s2);

		SATBox(const Laser& laser);
		SATBox(const Player& player, const Laser& laser);

		bool collide(const Player& player) const;
		bool collide(const Laser& laser) const;

		Pointf topLeft;
		Pointf topRight;
		Pointf bottomLeft;
		Pointf bottomRight;
	};
}

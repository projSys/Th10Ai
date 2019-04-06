#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	class Player;

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
	class Laser :
		public Entity
	{
	public:
		//Laser() :
		//	arc() {}
		//Laser(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
		//	Entity(x0, y0, dx0, dy0, width0, height0), arc() {}

		bool collide(const Player& player) const;
		bool collide(const Player& player, float_t frame) const;

		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;

		float_t arc;
	};

	class SATBox
	{
	public:
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

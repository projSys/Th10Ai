#pragma once

#include "TH10Bot/Entity.h"

namespace th
{
	class Player;

	// ����ṹ
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
		Laser advance(const Pointf& pos) const;
		Laser advance(float_t frame) const;
		bool collide(const Player& player) const;

		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;

		float_t arc;
	};

	class SATBox
	{
	public:
		static Pointf Rotate(const Pointf& P, const Pointf& C, float_t radianC);
		static bool Collide(float_t c1, float_t s1, float_t c2, float_t s2);

		Pointf topLeft;
		Pointf topRight;
		Pointf bottomLeft;
		Pointf bottomRight;
	};

	class LaserBox :
		public SATBox
	{
	public:
		LaserBox(const Laser& laser);

		bool collide(const Player& player) const;
	};

	class PlayerBox :
		public SATBox
	{
	public:
		PlayerBox(const Player& player, const Laser& laser);

		bool collide(const Laser& laser) const;
	};
}

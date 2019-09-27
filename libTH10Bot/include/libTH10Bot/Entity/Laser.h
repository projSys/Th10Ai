#pragma once

#include "libTH10Bot/Entity/Entity.h"

namespace th
{
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
		Laser();

		bool collide(const Entity& other) const;
		std::pair<bool, float_t> willCollideWith(const Entity& other) const;

		Pointf getTopLeft() const;
		Pointf getTopRight() const;
		Pointf getBottomLeft() const;
		Pointf getBottomRight() const;

		float_t arc;
	};

	class SatBox
	{
	public:
		static Pointf Rotate(const Pointf& P, const Pointf& C, float_t radianC);
		static bool Collide(float_t p1, float_t s1, float_t p2, float_t s2);

		Pointf topLeft;
		Pointf topRight;
		Pointf bottomLeft;
		Pointf bottomRight;
	};

	class LaserBox :
		public SatBox
	{
	public:
		LaserBox(const Laser& laser);

		bool collide(const Entity& other) const;
	};

	class EntityBox :
		public SatBox
	{
	public:
		EntityBox(const Entity& entity, const Laser& laser);

		bool collide(const Laser& other) const;
	};
}

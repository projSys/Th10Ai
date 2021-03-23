#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Entity.h"

namespace th
{
	// ¼¤¹â½á¹¹
	// (x,y)
	// +-*-+
	// |   |
	// |   |
	// |   |
	// |   | h
	// |   |
	// |   |
	// |   |
	// +---+
	//   w
	class Laser :
		public Entity
	{
	public:
		Laser();

		bool collide(const Entity& other) const;
		std::pair<bool, float_t> willCollideWith(const Entity& other) const;

		vec2 getLeftTop() const;
		vec2 getRightTop() const;
		vec2 getLeftBottom() const;
		vec2 getRightBottom() const;

		float_t arc;
	};

	class SatBox
	{
	public:
		vec2 leftTop;
		vec2 rightTop;
		vec2 leftBottom;
		vec2 rightBottom;
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

#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Math/Vector2.h"
#include "Th10Hook/Entity.h"

namespace th
{
	// ����ṹ
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
	class Laser
	{
	public:
		Laser();
		Laser(const vec2& pos0, const vec2& delta0, const vec2& size0, float_t arc0);

		float_t distance(const Entity& other) const;

		bool collide(const Entity& other) const;
		vec2 getFootPoint(const Entity& other) const;
		bool willCollideWith(const Entity& other) const;

		bool isHolding() const;
		vec2 getNextPos() const;
		void advance(float_t frame);

		vec2 getCenter() const;
		vec2 getLeftTop() const;
		vec2 getRightTop() const;
		vec2 getLeftBottom() const;
		vec2 getRightBottom() const;
		float_t getRadian() const;

		vec2 pos;
		vec2 delta;
		vec2 size;
		float_t arc;
	};
}

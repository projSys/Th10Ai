#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Math/Vector2.h"
#include "Th10Hook/Math/OBB.h"

namespace th
{
	class Entity;

	// ¼¤¹â½á¹¹
	//  (x,y)
	// +--*--+
	// |     |
	// |     |
	// |     |
	// |     | h
	// |     |
	// |     |
	// |     |
	// +-----+
	//    w
	class Laser
	{
	public:
		Laser();
		Laser(const vec2& pos0, const vec2& delta0, const vec2& size0, float_t arc0);

		void updateExtra();
		bool collide(const Entity& other) const;
		bool willCollideWith(const Entity& other) const;

		float_t distance(const Entity& other) const;
		bool isHolding() const;
		void advance(float_t frame);

		vec2 getLeftTop() const;
		vec2 getRightTop() const;
		vec2 getLeftBottom() const;
		vec2 getRightBottom() const;
		float_t getRadian() const;

		vec2 pos;
		vec2 delta;
		vec2 size;
		float_t arc;

		OBB obb;
		vec2 deltaV;
	};
}

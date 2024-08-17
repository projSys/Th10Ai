#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/OBB.h"
#include "Th10Base/Th10Raws.h"

namespace th
{
	class Entity;

	// 激光结构
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
	class Laser :
		public OBB
	{
	public:
		Laser();
		explicit Laser(const LaserRaw* raw);

		void updateExtra();
		bool willCollideWith(const AABB& other) const;

		vec2::value_t distance(const Entity& entity) const;
		void setPosition(const vec2& pos0);
		void advance(int_t frame);

	private:
		vec2 getTopLeft() const;
		vec2 getTopRight() const;
		vec2 getBottomLeft() const;
		vec2 getBottomRight() const;
		float_t getRadian() const;
		void updateOBB();

	public:
		vec2 pos;
		vec2 delta;
		vec2 size;
		float_t arc;

		vec2 deltaV;
		Projection projV;
	};
}

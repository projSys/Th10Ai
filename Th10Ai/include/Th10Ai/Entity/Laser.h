#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/OBB.h"

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

		void updateExtra();
		bool willCollideWith(const AABB& other) const;

		float_t distance(const Entity& entity) const;
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
		vec2 m_pos;
		vec2 m_delta;
		vec2 m_size;
		float_t m_arc;

		vec2 m_deltaV;
		Projection m_projV;
	};
}

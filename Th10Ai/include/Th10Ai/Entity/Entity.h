#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/AABB.h"

namespace th
{
	// 实体结构
	// +-------+
	// | (x,y) |
	// |   +   | h
	// |       |
	// +-------+
	//     w
	class Entity :
		public AABB
	{
	public:
		Entity();

		void updateExtra();
		//bool collide(const Entity& other) const;
		bool willCollideWith(const AABB& other) const;

		float_t distance(const Entity& other) const;
		void setPosition(const vec2& pos0);
		void advance(int_t frame);

	protected:
		vec2 getTopLeft() const;
		vec2 getTopRight() const;
		vec2 getBottomLeft() const;
		vec2 getBottomRight() const;
		void updateAABB();

	public:
		vec2 m_pos;			// 坐标在中心点
		vec2 m_delta;
		vec2 m_size;

		vec2 m_deltaV;
		Projection m_projV;
	};
}

#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/AABB.h"
#include "Th10Ai/Entity/Projection.h"

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
		Entity(const vec2& pos0, const vec2& delta0, const vec2& size0);

		void updateExtra();
		//bool collide(const Entity& other) const;
		bool willCollideWith(const AABB& other) const;

		float_t distance(const Entity& other) const;
		void advance(int_t frame);

	protected:
		vec2 getTopLeft() const;
		vec2 getTopRight() const;
		vec2 getBottomLeft() const;
		vec2 getBottomRight() const;
		void updateAABB();

	public:
		//int_t id;
		//int_t type;

		vec2 pos;			// 坐标在中心点
		vec2 delta;
		vec2 size;

		vec2 deltaV;
		Projection projV;
	};
}

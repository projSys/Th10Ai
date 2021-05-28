#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Math/Vector2.h"
#include "Th10Hook/Math/AABB.h"
#include "Th10Hook/Math/Projection.h"

namespace th
{
	// 实体结构
	// +-------+
	// | (x,y) |
	// |   +   | h
	// |       |
	// +-------+
	//     w
	class Entity
	{
	public:
		Entity();
		Entity(const vec2& pos0, const vec2& delta0, const vec2& size0);

		void updateExtra();
		bool collide(const Entity& other) const;
		bool willCollideWith(const Entity& other) const;

		float_t distance(const Entity& other) const;
		bool isHolding() const;
		void advance(int_t frame);

		vec2 getTopLeft() const;
		vec2 getTopRight() const;
		vec2 getBottomLeft() const;
		vec2 getBottomRight() const;

		//int_t id;
		//int_t type;

		vec2 pos;			// 坐标在中心点
		vec2 delta;
		vec2 size;

		// 只在willCollideWith()里使用，基于每帧的原始坐标
		AABB aabb;
		vec2 deltaV;
		Projection projV;
	};
}

#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Math/Vector2.h"

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

		float_t distance(const Entity& other) const;

		bool collide(const Entity& other) const;
		bool collide(const Entity& other, float_t epsilon) const;
		bool collide2(const Entity& other) const;
		bool collide3(const Entity& other, float_t frame) const;
		bool willCollideWith(const Entity& other) const;

		bool isHolding() const;
		vec2 getNextPos() const;
		void advance(float_t frame);

		vec2 getLeftTop() const;
		vec2 getRightTop() const;
		vec2 getLeftBottom() const;
		vec2 getRightBottom() const;

		//int_t id;
		//int_t type;

		vec2 pos;			// 坐标在中心点
		vec2 delta;
		vec2 size;

		vec2 deltaV;
	};
}

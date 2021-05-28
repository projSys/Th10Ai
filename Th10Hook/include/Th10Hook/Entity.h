#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Math/Vector2.h"
#include "Th10Hook/Math/AABB.h"
#include "Th10Hook/Math/Projection.h"

namespace th
{
	// ʵ��ṹ
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

		vec2 pos;			// ���������ĵ�
		vec2 delta;
		vec2 size;

		// ֻ��willCollideWith()��ʹ�ã�����ÿ֡��ԭʼ����
		AABB aabb;
		vec2 deltaV;
		Projection projV;
	};
}

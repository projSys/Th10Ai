#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Math/Vector2.h"

namespace th
{
	class Entity;

	class AABB
	{
	public:
		AABB() = default;
		explicit AABB(const Entity& entity);

		void update(const Entity& entity);
		bool collide(const AABB& other) const;

		vec2 leftTop;
		vec2 rightTop;
		vec2 leftBottom;
		vec2 rightBottom;
		vec2 axisX;	// ͶӰ�õĶԳ��ᣬ��λ����
		vec2 axisY;
	};
}

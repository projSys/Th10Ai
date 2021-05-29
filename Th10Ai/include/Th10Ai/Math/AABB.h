#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Math/BB.h"

namespace th
{
	class Entity;

	class AABB :
		public BB
	{
	public:
		AABB() = default;
		explicit AABB(const Entity& entity);

		void update(const Entity& entity);
		bool collide(const AABB& other) const;

		vec2 axisX;
		vec2 axisY;
	};
}

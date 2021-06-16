#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/BB.h"

namespace th
{
	// ������Χ��
	class AABB :
		public BB
	{
	public:
		// ͶӰ�ᣬ��λ����
		static constexpr vec2 PROJ_AXIS_X = vec2(_F(1.0), _F(0.0));
		static constexpr vec2 PROJ_AXIS_Y = vec2(_F(0.0), _F(1.0));

		Projection projectX() const;
		Projection projectY() const;
		bool collide(const AABB& other) const;
	};
}

#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/Vector2.h"
#include "Th10Base/Projection.h"

namespace th
{
	// 矩形包围盒
	class BB
	{
	public:
		// 投影，把2维降成1维，投影轴必须是单位向量
		Projection projectByNormalized(const vec2& projAxis) const;
		Projection project(const vec2& projAxis) const;

		// 平移
		void translate(const vec2& offset);

	public:
		vec2 topLeft;
		vec2 topRight;
		vec2 bottomLeft;
		vec2 bottomRight;
	};
}

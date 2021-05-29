#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Math/BB.h"

namespace th
{
	class Laser;
	class AABB;

	class OBB :
		public BB
	{
	public:
		OBB() = default;
		explicit OBB(const Laser& laser);

		void update(const Laser& laser);
		bool collide(const AABB& other) const;

		vec2 axisX;	// 投影用的对称轴，单位向量
		vec2 axisY;
	};
}

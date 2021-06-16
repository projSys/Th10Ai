#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/BB.h"

namespace th
{
	class AABB;

	// �����Χ��
	class OBB :
		public BB
	{
	public:
		bool collide(const AABB& other) const;

	protected:
		vec2 m_projAxisX;	// ͶӰ�ᣬ��λ����
		vec2 m_projAxisY;
	};
}

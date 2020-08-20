#include "Th10Ai/Common.h"
#include "Th10Ai/Utils.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <cmath>
#include <Base/TypeTraits.h>

namespace th
{
	float_t Utils::CalcDistance(const Pointf& A, const Pointf& B)
	{
		float_t dx = A.x - B.x;
		float_t dy = A.y - B.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// ���Ҷ���
	float_t Utils::CalcAngle(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		float_t AB = CalcDistance(A, B);
		float_t AC = CalcDistance(A, C);
		float_t BC = CalcDistance(B, C);
		if (TypeTraits<float_t>::IsEqual(AB, 0.0f)
			|| TypeTraits<float_t>::IsEqual(AC, 0.0f))
			return -1.0f;

		float_t cosA = (AB * AB + AC * AC - BC * BC) / (2.0f * AB * AC);
		if (cosA < -1.0f)
			cosA = -1.0f;
		if (cosA > 1.0f)
			cosA = 1.0f;

		float_t radianA = std::acos(cosA);
		// �Ƕ� = ���� * 180 / PI
		return radianA * 180.0f / static_cast<float_t>(M_PI);
	}

	// ���ȣ���һϵ��k����ֱ�ߵ������յ�ֱ�ΪA��x1�� y1����B��x2�� y2����ֱ����һ��ΪC��x0�� y0��������ΪD������ k = |AD| / |AB|��
	// �� k * AB = AD = AC + CD���� AB * CD = 0������ k * AB * AB = AC * AB���� k = AC * AB / ��AB * AB����
	// �������꣬���� k = ((x0 - x1) * (x2 - x1) + (y0 - y1) * (y2 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	// �� x = x1 + k * (x2 - x1); y = y1 + k * (y2 - y1);
	FootPoint Utils::CalcFootPoint(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		FootPoint footPoint = {};

		float_t dxBA = B.x - A.x;
		float_t dyBA = B.y - A.y;
		if (TypeTraits<float_t>::IsEqual(dxBA, 0.0f)
			&& TypeTraits<float_t>::IsEqual(dyBA, 0.0f))
		{
			footPoint.k = 0.0f;
			footPoint.pos = A;
			return footPoint;
		}

		float_t dxCA = C.x - A.x;
		float_t dyCA = C.y - A.y;
		footPoint.k = (dxCA * dxBA + dyCA * dyBA) / (dxBA * dxBA + dyBA * dyBA);
		footPoint.pos.x = A.x + footPoint.k * dxBA;
		footPoint.pos.y = A.y + footPoint.k * dyBA;
		return footPoint;
	}
}

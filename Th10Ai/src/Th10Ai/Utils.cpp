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

	// 余弦定理
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
		// 角度 = 弧度 * 180 / PI
		return radianA * 180.0f / static_cast<float_t>(M_PI);
	}

	// 首先，求一系数k：设直线的起点和终点分别为A（x1， y1）、B（x2， y2），直线外一点为C（x0， y0），垂足为D；并设 k = |AD| / |AB|。
	// 则 k * AB = AD = AC + CD，又 AB * CD = 0；所以 k * AB * AB = AC * AB，故 k = AC * AB / （AB * AB）。
	// 带入坐标，即得 k = ((x0 - x1) * (x2 - x1) + (y0 - y1) * (y2 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	// 则 x = x1 + k * (x2 - x1); y = y1 + k * (y2 - y1);
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

#include "TH10Bot/Common.h"
#include "TH10Bot/MyMath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace th
{
	// 勾股定理
	float_t MyMath::Distance(const Pointf& A, const Pointf& B)
	{
		float_t dx = A.x - B.x;
		float_t dy = A.y - B.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// 余弦定理
	float_t MyMath::Angle(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		float_t AB = Distance(A, B);
		float_t AC = Distance(A, C);
		float_t BC = Distance(B, C);
		float_t cosA = (AB * AB + AC * AC - BC * BC) / (2.0f * AB * AC);
		float_t radian = std::acos(cosA);
		// 角度 = 弧度 * 180 / PI
		return radian * 180.0f / static_cast<float_t>(M_PI);
	}

	// 点到直线的垂足
	Pointf MyMath::FootPoint(const Pointf& A, const Pointf& B, const Pointf& P)
	{
		float_t dx = B.x - A.x;
		float_t dy = B.y - A.y;
		float_t u = ((P.x - A.x) * dx + (P.y - A.y) * dy) / (dx * dx + dy * dy);
		return Pointf(A.x + dx * u, A.y + dy * u);
	}
}

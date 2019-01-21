#include "TH10Bot/Common.h"
#include "TH10Bot/MyMath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace th
{
	float_t MyMath::Distance(const Pointf& A, const Pointf& B)
	{
		float_t dx = A.x - B.x;
		float_t dy = A.y - B.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// ���Ҷ���
	float_t MyMath::Angle(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		float_t AB = Distance(A, B);
		float_t AC = Distance(A, C);
		float_t BC = Distance(B, C);
		float_t cosA = (AB * AB + AC * AC - BC * BC) / (2.0f * AB * AC);
		float_t radian = std::acos(cosA);
		// �Ƕ� = ���� * 180 / PI
		return static_cast<float_t>(radian * 180.0f / M_PI);
	}
}

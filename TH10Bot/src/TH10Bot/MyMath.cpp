#include "TH10Bot/Common.h"
#include "TH10Bot/MyMath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace th
{
	// ���ɶ���
	float_t MyMath::GetDistance(const Pointf& A, const Pointf& B)
	{
		float_t dx = A.x - B.x;
		float_t dy = A.y - B.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// ���Ҷ���
	float_t MyMath::GetAngle(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		float_t AB = GetDistance(A, B);
		float_t AC = GetDistance(A, C);
		float_t BC = GetDistance(B, C);
		float_t cosA = (AB * AB + AC * AC - BC * BC) / (2.0f * AB * AC);
		float_t radianA = std::acos(cosA);
		// �Ƕ� = ���� * 180 / PI
		return radianA * 180.0f / static_cast<float_t>(M_PI);
	}

	// �㵽ֱ�ߵĴ���
	FootPoint MyMath::GetFootPoint(const Pointf& A, const Pointf& B, const Pointf& P)
	{
		float_t dx = B.x - A.x;
		float_t dy = B.y - A.y;
		float_t ratio = ((P.x - A.x) * dx + (P.y - A.y) * dy) / (dx * dx + dy * dy);
		return { A.x + dx * ratio, A.y + dy * ratio, ratio };
	}

	// ƽ���У�һ����(x, y)�������(cx, cy)˳ʱ����תa�Ⱥ������
	// xx = (x - cx) * cos(-a) - (y - cy) * sin(-a) + cx;
	// yy = (x - cx) * sin(-a) + (y - cy) * cos(-a) + cy;
	// ƽ���У�һ����(x, y)�������(cx, cy)��ʱ����תa�Ⱥ������
	// xx = (x - cx) * cos(a) - (y - cy) * sin(a) + cx;
	// yy = (x - cx) * sin(a) + (y - cy) * cos(a) + cy;
	Pointf MyMath::Rotate(const Pointf& P, const Pointf& C, float_t radianC)
	{
		float_t dx = P.x - C.x;
		float_t dy = P.y - C.y;
		float_t sinC = std::sin(radianC);
		float_t cosC = std::cos(radianC);
		return Pointf(dx * cosC - dy * sinC + C.x, dx * sinC + dy * cosC + C.y);
	}
}

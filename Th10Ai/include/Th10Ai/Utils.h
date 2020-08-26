#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	struct FootPoint
	{
		float_t k;
		Pointf pos;
	};

	class Utils
	{
	public:
		static float_t CalcDistance(const Pointf& A, const Pointf& B);
		// AB与AC的夹角
		static float_t CalcAngle(const Pointf& A, const Pointf& B, const Pointf& C);
		// 点C到线段AB的垂足
		static FootPoint CalcFootPoint(const Pointf& A, const Pointf& B, const Pointf& C);
	};
}

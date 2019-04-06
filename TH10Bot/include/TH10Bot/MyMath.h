#pragma once

namespace th
{
	class MyMath
	{
	public:
		static float_t GetDistance(const Pointf& A, const Pointf& B);
		static float_t GetAngle(const Pointf& A, const Pointf& B, const Pointf& C);
		static FootPoint GetFootPoint(const Pointf& A, const Pointf& B, const Pointf& P);
		static Pointf Rotate(const Pointf& P, const Pointf& C, float_t radianC);
	};
}

#pragma once

namespace th
{
	class MyMath
	{
	public:
		static float_t GetAngle(const Pointf& A, const Pointf& B, const Pointf& C);
		static Pointf Rotate(const Pointf& P, const Pointf& C, float_t radianC);
	};
}

#pragma once

namespace th
{
	class MyMath
	{
	public:
		static float_t Distance(const Pointf& A, const Pointf& B);
		static float_t Angle(const Pointf& A, const Pointf& B, const Pointf& C);
		static Pointf FootPoint(const Pointf& A, const Pointf& B, const Pointf& P);
	};
}

#include "Th10Ai/Entity/Projection.h"

namespace th
{
	Projection::Projection() :
		m_min(),
		m_max()
	{
	}

	Projection::Projection(float_t min, float_t max) :
		m_min(min),
		m_max(max)
	{
	}

	bool Projection::overlap(const Projection& other) const
	{
		//return !(m_max < other.m_min || other.m_max < m_min);
		return m_max > other.m_min && other.m_max > m_min;
	}
}

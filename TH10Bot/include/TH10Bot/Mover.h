#pragma once

namespace th
{
	class Mover
	{
	public:
		Mover(Direction fromDir);

		bool hasNext() const;
		Direction next();

	private:
		Direction m_fromDir;
		int_t m_i;
	};
}

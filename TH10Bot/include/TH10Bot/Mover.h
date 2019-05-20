#pragma once

namespace th
{
	class Mover
	{
	public:
		Mover(Direction targetDir);

		bool hasNext() const;
		Direction next();
		int_t getSize() const;

	private:
		Direction m_targetDir;
		int_t m_i;
	};
}

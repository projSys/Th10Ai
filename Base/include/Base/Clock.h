#pragma once

#include "Base/Common.h"

#include "Base/Types.h"

namespace base
{
	class Clock
	{
	public:
		Clock();

		void update();
		int64_t getTime() const;

	private:
		int64_t m_time;
	};
}

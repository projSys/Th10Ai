#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Status.h"

namespace th
{
	class Listener
	{
	public:
		virtual ~Listener() = default;

		virtual void onStatusUpdate(const SharedStatus& status) = 0;
	};
}

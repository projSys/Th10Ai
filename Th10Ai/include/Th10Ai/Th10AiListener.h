#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Status.h"

namespace th
{
	class Th10AiListener
	{
	public:
		virtual ~Th10AiListener() = default;
		virtual void onStatusUpdate(const Status& status) = 0;
	};
}

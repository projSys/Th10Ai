#pragma once

#include "Th10Ai/Common.h"

#include <thread>
#include <atomic>
#include <optional>
#include <Base/Time.h>

#include "Th10Ai/Config.h"
#include "Th10Ai/Th10Hook.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"

namespace th
{
	class Listener
	{
	public:
		virtual ~Listener() = default;

		virtual void onStatusUpdate(const SharedStatus& status) = 0;
	};
}

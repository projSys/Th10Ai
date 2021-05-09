#pragma once

#include "Base/Common.h"

#include <chrono>

namespace base
{
	namespace chrono = std::chrono;
	using namespace std::chrono_literals;

	using Time = chrono::steady_clock::duration;

	class Clock
	{
	public:
		static Time Now();
	};
}

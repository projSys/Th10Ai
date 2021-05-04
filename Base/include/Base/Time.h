#pragma once

#include "Base/Common.h"

#include <chrono>

#include "Base/Types.h"

namespace base
{
	namespace chrono = std::chrono;

	class Time
	{
	public:
		static Time Now();

		Time() = default;
		explicit Time(const chrono::steady_clock::duration& dur);

		int64_t getNanoSeconds() const;
		int64_t getMicroSeconds() const;
		int64_t getMilliSeconds() const;
		int64_t getSeconds() const;
		int64_t getMinutes() const;
		int64_t getHours() const;

	private:
		chrono::steady_clock::duration m_dur;
	};
}

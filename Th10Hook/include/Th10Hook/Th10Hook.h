#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	class Th10Listener
	{
	public:
		virtual ~Th10Listener() = default;
	};

	// https://github.com/binvec/TH10_DataReversing
	class Th10Hook
	{
	public:
		Th10Hook(Th10Listener* listener);

	private:
		template <typename T>
		static T ReadMemory(uint_t address)
		{
			return *reinterpret_cast<T*>(address);
		}

		Th10Listener* m_listener;
	};
}

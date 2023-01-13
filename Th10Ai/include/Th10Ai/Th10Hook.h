#pragma once

#include "Th10Ai/Common.h"

#include <Base/Time.h>
#include <Th10Base/SharedMemory.h>
#include <Th10Base/SharedData.h>

namespace th
{
	class Th10Hook
	{
	public:
		Th10Hook();
		~Th10Hook();

		bool waitInit(const Time& timeout = Time(3000));
		bool isActive() const;
		void setActive(bool active);
		HWND getWindow() const;

		bool waitUpdate();
		const SharedStatus& getReadableStatus() const;

		void notifyInput();
		Input& getWritableInput();

	private:
		SharedMemory m_sharedMemory;
		SharedData* m_sharedData;
	};
}

#pragma once

#include <array>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#include "GraphCap/Input.h"

namespace gc
{
	namespace bip = boost::interprocess;

	struct DI8SharedData
	{
		bip::interprocess_mutex keyMutex;
		KeyState keyState[256];
	};

	class DI8Input
	{
	public:
		DI8Input();

		void clear();
		void keyClear(Key key);
		void keyPress(Key key);
		void keyRelease(Key key);
		bool isKeyPressed(Key key) const;
		void commit();

	private:
		static const std::array<BYTE, KEY_MAXCOUNT> KEY_MAP;

		bip::managed_shared_memory m_memory;
		DI8SharedData* m_data;
		KeyState m_keyState[256];
	};
}

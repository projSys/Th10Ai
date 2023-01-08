#include "Th10Base/SharedStatus.h"

namespace th
{
	SharedStatus::SharedStatus(SharedMemory& sharedMemory) :
		m_voidAlloc(sharedMemory.get_segment_manager()),
		globalVar{},
		items(m_voidAlloc),
		enemies(m_voidAlloc),
		bullets(m_voidAlloc),
		lasers(m_voidAlloc)
	{
		items.reserve(ITEM_MAX_COUNT);
		enemies.reserve(200);
		bullets.reserve(BULLET_MAX_COUNT);
		lasers.reserve(200);
	}
}

#pragma once

#include <d3d9.h>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

#include "GraphCap/Image.h"

namespace gc
{
	namespace bip = boost::interprocess;

	struct D3D9SharedData
	{
		bip::interprocess_mutex presentMutex;
		bip::interprocess_condition presentCond;
	};

	class D3D9Capturer
	{
	public:
		D3D9Capturer();

		bool capture(Image& image, time_t timeout = 17);

	private:
		bip::managed_shared_memory m_memory;
		D3D9SharedData* m_data;
	};
}

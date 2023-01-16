#include "Th10Base/SharedData.h"

namespace th
{
	SharedData::SharedData(SharedMemory& sharedMemory) :
		inited(false),
		active(false),
		window(nullptr),
		statusUpdated(false),
		exit(false),
		writableStatus(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(bip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		swappableStatus(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(bip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		readableStatus(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(bip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		inputUpdated(true),	// 跳过第一帧输入
		writableInput(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedInput>(bip::anonymous_instance)(),
			sharedMemory)),
		swappableInput(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedInput>(bip::anonymous_instance)(),
			sharedMemory)),
		readableInput(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedInput>(bip::anonymous_instance)(),
			sharedMemory))
	{
	}
}

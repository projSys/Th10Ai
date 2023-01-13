#include "Th10Base/SharedData.h"

namespace th
{
	SharedData::SharedData(SharedMemory& sharedMemory) :
		inited(false),
		active(false),
		window(nullptr),
		statusUpdated(false),
		exit(false),
		writableStatus(ip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(ip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		swappableStatus(ip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(ip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		readableStatus(ip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(ip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		inputUpdated(true),	// 跳过第一帧输入
		writableInput(ip::make_managed_unique_ptr(
			sharedMemory.construct<Input>(ip::anonymous_instance)(),
			sharedMemory)),
		swappableInput(ip::make_managed_unique_ptr(
			sharedMemory.construct<Input>(ip::anonymous_instance)(),
			sharedMemory)),
		readableInput(ip::make_managed_unique_ptr(
			sharedMemory.construct<Input>(ip::anonymous_instance)(),
			sharedMemory))
	{
	}
}

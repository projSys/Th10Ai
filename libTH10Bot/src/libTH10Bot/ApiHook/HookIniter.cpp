#include "libTH10Bot/Common.h"
#include "libTH10Bot/ApiHook/HookIniter.h"

#include <MinHook.h>

namespace th
{
	HookIniter::HookIniter()
	{
		MH_STATUS status = MH_Initialize();
		if (status != MH_OK)
			THROW_BASE_EXCEPTION(Exception() << err_no(status) << err_str(MH_StatusToString(status)));
	}

	HookIniter::~HookIniter()
	{
		MH_Uninitialize();
	}
}

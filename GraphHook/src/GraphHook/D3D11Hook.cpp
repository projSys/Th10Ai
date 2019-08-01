#include "GraphHook/Common.h"
#include "GraphHook/D3D11Hook.h"

#include <Base/ScopeGuard.h>

namespace gh
{
	D3D11Hook::D3D11Hook() :
		Singleton(this)
	{
	}
}

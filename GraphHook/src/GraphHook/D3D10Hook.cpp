#include "GraphHook/Common.h"
#include "GraphHook/D3D10Hook.h"

#include <Base/ScopeGuard.h>

namespace gh
{
	D3D10Hook::D3D10Hook() :
		Singleton(this)
	{
	}
}

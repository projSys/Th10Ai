#pragma once

#include <atlbase.h>
#include <Base/Singleton.h>

#include "GraphHook/HookFunc.h"

namespace gh
{
	class D3D10Hook :
		private Singleton<D3D10Hook>
	{
	public:
		D3D10Hook();
	};
}

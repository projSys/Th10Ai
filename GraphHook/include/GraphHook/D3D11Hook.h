#pragma once

#include <atlbase.h>
#include <Base/Singleton.h>

#include "GraphHook/HookFunc.h"

namespace gh
{
	class D3D11Hook :
		private Singleton<D3D11Hook>
	{
	public:
		D3D11Hook();
	};
}

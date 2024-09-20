#pragma once

#include "Base/Common.h"

#include "Base/Streamable.h"

namespace base
{
	// 只能在catch块里使用
	class Catcher :
		public OStreamable
	{
	public:
		virtual void toStream(std::ostream& out) const override;
	};
}

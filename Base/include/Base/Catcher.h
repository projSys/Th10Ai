#pragma once

#include "Base/Common.h"

#include "Base/Streamable.h"

namespace base
{
	// 只能在catch块里调用
	class Catcher :
		public OStreamable
	{
	public:
		virtual void toStream(std::ostream& os) const override;
	};
}

#pragma once

#include "Base/Common.h"

#include "Base/Streamable.h"

namespace base
{
	// ֻ����catch�������
	class Catcher :
		public OStreamable
	{
	public:
		virtual void toStream(std::ostream& os) const override;
	};
}

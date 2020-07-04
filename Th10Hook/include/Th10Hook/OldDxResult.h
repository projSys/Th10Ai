#pragma once

#include "Base/Exception.h"

namespace th
{
	class OldDxResult :
		public Exception
	{
	public:
		OldDxResult(HRESULT result);

		virtual void print(ostream& os) const override;

	private:
		HRESULT m_result;
	};
}

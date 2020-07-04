#pragma once

#include "Base/Exception.h"

namespace win
{
	class DxResult :
		public Exception
	{
	public:
		explicit DxResult(HRESULT result);

		virtual void print(ostream& os) const override;

	private:
		HRESULT m_result;
	};
}

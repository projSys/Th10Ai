#pragma once

#include "Base/Exception.h"

namespace win
{
	class DirectXResult :
		public Exception
	{
	public:
		DirectXResult(HRESULT result);

		virtual void print(std::ostream& os) const override;

	private:
		HRESULT m_result;
	};
}

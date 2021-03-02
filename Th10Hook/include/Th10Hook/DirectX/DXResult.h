#pragma once

#include "Th10Hook/Common.h"

#include <Base/Exception.h>

namespace th
{
	class DXResult :
		public Exception
	{
	public:
		explicit DXResult(HRESULT result, const char* str = nullptr);
		DXResult(HRESULT result, const std::string& str);

		virtual void printTo(std::ostream& os) const override;

	private:
		HRESULT m_result;
		bool m_customized;
	};
}

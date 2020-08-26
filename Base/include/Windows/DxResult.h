#pragma once

#include "Windows/Common.h"

#include "Base/Exception.h"

namespace win
{
	class DxResult :
		public Exception
	{
	public:
		explicit DxResult(HRESULT result);
		DxResult(HRESULT result, const char* str);
		DxResult(HRESULT result, const std::string& str);

		virtual void print(std::ostream& os) const override;

	private:
		HRESULT m_result;
		bool m_custom;
	};
}

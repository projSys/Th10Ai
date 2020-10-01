#pragma once

#include "Base/Windows/Common.h"

#include "Base/Exception.h"

namespace base
{
	namespace win
	{
		class DxResult :
			public Exception
		{
		public:
			explicit DxResult(HRESULT result, const char* ptr = nullptr);
			DxResult(HRESULT result, const std::string& str);

			virtual void printTo(std::ostream& os) const override;

		private:
			HRESULT m_result;
			bool m_customized;
		};
	}
}

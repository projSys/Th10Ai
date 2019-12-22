#pragma once

#include "Base/Exception.h"

namespace win
{
	class DirectXResult :
		public Exception
	{
	public:
		DirectXResult(HRESULT result,
			const char* func, const char* file, uint_t line);

		virtual void print(std::ostream& os) const override;

		HRESULT getResult() const;

	private:
		HRESULT m_result;
	};

#define THROW_DIRECTX_RESULT(result) \
	throw win::DirectXResult(result, __func__, __FILE__, __LINE__)
}

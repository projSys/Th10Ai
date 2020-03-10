#pragma once

#include "Base/Exception.h"

namespace win
{
	enum class DXVER
	{
		DD7,
		DI8,
		D3D9,
		D3D10,
		D3D11,
		D3D12
	};

	class DirectXResult :
		public Exception
	{
	public:
		DirectXResult(HRESULT hr, DXVER version);

		virtual void print(std::ostream& os) const override;

	private:
		HRESULT m_hr;
		DXVER m_version;
	};
}

#include "Windows/Common.h"
#include "Windows/DirectXResult.h"

#include <iomanip>

#include "Windows/Utils.h"
// https://walbourn.github.io/wheres-dxerr-lib/
#include "Windows/dxerr.h"

namespace win
{
	DirectXResult::DirectXResult(HRESULT hr, DXVER version) :
		m_hr(hr),
		m_version(version)
	{
	}

	void DirectXResult::print(std::ostream& os) const
	{
		if (m_version < DXVER::D3D10)
		{
			WCHAR buffer[4096] = {};
			DXGetErrorDescriptionW(m_hr, buffer, 4095);
			os << '[' << String::WideToUtf8(DXGetErrorStringW(m_hr)) << ']'
				<< String::WideToUtf8(buffer) << '\n';
		}
		else
		{
			std::ios oldState(nullptr);
			oldState.copyfmt(os);
			os << "[0x" << std::hex << std::uppercase << std::setw(8)
				<< std::setfill('0') << m_hr << ']';
			os.copyfmt(oldState);
			os << Utils::GetErrorDescription(m_hr) << '\n';
		}
		printSourceLocation(os);
		printStackTrace(os);
	}
}

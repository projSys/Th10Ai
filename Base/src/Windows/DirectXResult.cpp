#include "Windows/Common.h"
#include "Windows/DirectXResult.h"

#include <iomanip>
#include <boost/locale.hpp>

#include "Windows/Util.h"
// https://walbourn.github.io/wheres-dxerr-lib/
#include "Windows/dxerr.h"

namespace win
{
	namespace blc = boost::locale::conv;

	DirectXResult::DirectXResult(HRESULT hr, DXVER version) :
		Exception(nullptr),
		m_hr(hr),
		m_version(version)
	{
	}

	void DirectXResult::print(std::ostream& os) const
	{
		if (m_version < DXVER::D3D10)
		{
			// https://walbourn.github.io/wheres-dxerr-lib/
			WCHAR buffer[4096] = {};
			DXGetErrorDescriptionW(m_hr, buffer, 4095);
			os << '[' << blc::utf_to_utf<char>(DXGetErrorStringW(m_hr)) << ']'
				<< blc::utf_to_utf<char>(buffer) << '\n';
		}
		else
		{
			std::ios oldState(nullptr);
			oldState.copyfmt(os);
			os << "[0x" << std::hex << std::uppercase << std::setw(8)
				<< std::setfill('0') << m_hr << ']';
			os.copyfmt(oldState);
			os << Util::GetErrorDescription(m_hr) << '\n';
		}
		printSourceLocation(os);
		printStackTrace(os);
	}
}

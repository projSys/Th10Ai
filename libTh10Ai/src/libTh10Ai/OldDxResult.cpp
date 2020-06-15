#include "libTh10Ai/Common.h"
#include "libTh10Ai/OldDxResult.h"

#include <iomanip>

// https://walbourn.github.io/wheres-dxerr-lib/
#include "libTh10Ai/dxerr.h"

namespace th
{
	OldDxResult::OldDxResult(HRESULT result) :
		m_result(result)
	{
	}

	void OldDxResult::print(std::ostream& os) const
	{
		WCHAR buffer[4096] = {};
		DXGetErrorDescriptionW(m_result, buffer, 4095);
		os << '[' << String::WideToUtf8(DXGetErrorStringW(m_result)) << ']'
			<< String::WideToUtf8(buffer) << '\n';

		printSourceLocation(os);
		printStackTrace(os);
	}
}

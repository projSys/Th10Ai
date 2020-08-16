#include "Windows/Common.h"
#include "Windows/DxResult.h"

#include <iomanip>

#include "Windows/Apis.h"

namespace win
{
	DxResult::DxResult(HRESULT result) :
		m_result(result)
	{
	}

	void DxResult::print(std::ostream& os) const
	{
		std::ios oldState(nullptr);
		oldState.copyfmt(os);
		os << "[0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << m_result << ']';
		os.copyfmt(oldState);
		os << Apis::GetErrorDesc(m_result) << '\n';

		printSourceLocation(os);
		printStackTrace(os);
	}
}
